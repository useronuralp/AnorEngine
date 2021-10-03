#include "Core/Engine.h"
#include "Core/EntryPoint.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Scene/SceneSerializer.h"
#include "Utility/WindowsUtils.h"
#include "ExampleLayer.h"
#include "Panels/ContentBrowserPanel.h"

using namespace AnorEngine;
using namespace Graphics;


//Defined in texture.cpp
extern const std::filesystem::path g_AssetPath;
extern const std::filesystem::path g_ResourcesPath;

class AnorEditor : public Application
{
private:
	Ref<SceneHierarchyPanel>          m_SceneHierarchyPanel;
	Ref<ContentBrowserPanel>          m_ContentBrowserPanel;
	LayerStack						  m_LayerStack;
	Ref<OrthographicCamera>			  m_OrthoCamera;
	Ref<EditorCamera>				  m_EditorCamera;
	Ref<ExampleLayer>				  m_Layer;
	ViewportVariables				  m_ViewportVariables;
	float							  m_LastFrameRenderTime;
	bool							  m_Minimized = false;
	bool							  m_IsRuntime = false; 
public:
	AnorEditor(const char* appName)
		:Application(appName), m_OrthoCamera(std::make_shared<OrthographicCamera>(-1280.0f / 720.0f * (5), 1280.0f / 720.0f * (5), -1 * (5), 1 * (5)))
	{
		Input::EventHandler::SetTargetApplication(this); //Important to set this to the active Application else, you won't get your input processed.	
		//Camera////////////////////////////////////////
		m_EditorCamera = std::make_shared<EditorCamera>();
		//Layer Creation////////////////////////////////
		m_Layer = std::make_shared<ExampleLayer>(m_EditorCamera);
		//Panels////////////////////////////////
		m_ContentBrowserPanel = std::make_shared<ContentBrowserPanel>();
		m_SceneHierarchyPanel = std::make_shared<SceneHierarchyPanel>(m_Layer->GetScene());
		//Layer insertion/////////////////////////////////
		PushLayer(m_Layer);
	}
public:
	virtual void Run() override
	{
		while (!m_OpenGLWindow->IsClosed())
		{
			float deltaTime = DeltaTime();

			Renderer2D::RenderDirectionalLightShadowMap(m_Layer->GetScene());
			Renderer2D::RenderPointLightShadowMaps(m_Layer->GetScene());

			Renderer2D::PreRender();
			if (!m_Minimized) //We don't want to render if the window is minimized.
			{
				for (Ref<Layer> layer : m_LayerStack)
					Renderer2D::RenderScene(layer, m_IsRuntime, deltaTime);
			}
			Renderer2D::PostRender();

			//-----------------------ImGui Beginning-------------------------
			ImGuiBase::Begin(); 
			//ImGui::ShowDemoWindow();
			ImGuiBase::BeginDockSpace();
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New"))
						CreateScene();
					if (ImGui::MenuItem("Open..."))
						OpenScene();
					if (ImGui::MenuItem("Save As..."))
						SaveSceneAs();
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			ImGuiBase::EndDockSpace();



			ImGuiBase::BeginViewport(m_ViewportVariables);
			//First thing first, paste the framebuffer texture onto ImGui::Image.
			uint32_t texture = Renderer2D::GetMainFramebuffer()->GetColorAttachmentTextureID(0);
			ImGui::Image((void*)texture, { Renderer2D::GetMainFramebuffer()->GetDimensions().x, Renderer2D::GetMainFramebuffer()->GetDimensions().y }, { 0,1 }, { 1,0 });

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					if (std::wstring(path).find(L"Scenes\\") != std::wstring::npos)
					{
						OpenScene(g_AssetPath / path);
					}
					else
					{
						std::list<const wchar_t*> supportedFileTypes = { L".obj", L".fbx", L".gltf" };
						for (auto& type : supportedFileTypes)
						{
							if (std::wstring(path).find(type) != std::wstring::npos)
							{
								Ref<Model> model = std::make_shared<Model>(g_AssetPath / path);
								auto entity = m_Layer->GetScene()->CreateEntity("Unnamed Entity", "Model");
								Ref<Graphics::Material> defaultMaterial = std::make_shared<Graphics::Material>(Graphics::ShaderLibrary::GetShader("3DShader"));
								defaultMaterial->m_Properties.Ambient = 0.05f;
								defaultMaterial->m_Properties.Diffuse = 1.0f;
								defaultMaterial->m_Properties.Specular = 1.0f;
								entity.AddComponent<MeshRendererComponent>(defaultMaterial);
								entity.AddComponent<ModelRendererComponent>(model);
								break;
							}
						}
					}
				}
				ImGui::EndDragDropTarget();
			}

			m_SceneHierarchyPanel->OnImGuiRender();
			m_ContentBrowserPanel->OnImGuiRender();

			ImGuiBase::EndViewport();

			ImGui::Begin("Off Screen Render Target");
			// Continue here !! Give HDRBuffer its own shader and render the scene to it.
			int depthBuffer = 12;
			texture = Renderer2D::GetMainFramebuffer()->GetColorAttachmentTextureID();
			ImGui::Image((void*)depthBuffer, { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y - 50 }, { 0,1 }, { 1,0 });
			ImGui::End();

			ImGuiBase::End(); 
			//-----------------------ImGui END -------------------------------
			
			//Try to resize the framebuffer at the end. Otherwise it causes weird flickering problems.
			//Resizing the framebuffer if the ImGui panel happens to get resized. (Framebuffer and the panel sizes should be equal.)
			if (Renderer2D::GetMainFramebuffer()->GetDimensions().x != m_ViewportVariables.ViewportSize.x || Renderer2D::GetMainFramebuffer()->GetDimensions().y != m_ViewportVariables.ViewportSize.y)
			{
				Renderer2D::GetMainFramebuffer()->Resize(m_ViewportVariables.ViewportSize.x, m_ViewportVariables.ViewportSize.y);
				for (auto layerIterator = m_LayerStack.rbegin(); layerIterator != m_LayerStack.rend(); layerIterator++)
				{
					layerIterator->get()->OnResizeViewport(m_ViewportVariables.ViewportSize.x, m_ViewportVariables.ViewportSize.y);
				}
				m_EditorCamera->SetViewportSize(m_ViewportVariables.ViewportSize.x, m_ViewportVariables.ViewportSize.y);
			}

			m_EditorCamera->OnUpdate(deltaTime, m_ViewportVariables.ViewportHovered);
			m_OpenGLWindow->Update();
		}
	}

	virtual void OnEvent(Ref<Input::Event> e) override
		{
			//e->Log();
			//Handle events for editor camera.
			if (e->GetEventType() == Input::EventType::WindowResizeEvent)
			{
				for (auto layerIterator = m_LayerStack.rbegin(); layerIterator != m_LayerStack.rend(); layerIterator++)
				{
					//Sending the events in reverse order here
					layerIterator->get()->OnEvent(e);
				}
				auto castEvent = std::static_pointer_cast<Input::WindowResizeEvent>(e);
				if (castEvent->GetHeight() == 0 || castEvent->GetWidth() == 0)
					m_Minimized = true;
				else if (castEvent->GetHeight() > 0 || castEvent->GetWidth() > 0)
					m_Minimized = false;
			}
			if (!m_ViewportVariables.BlockEvents)
			{
				m_EditorCamera->OnEvent(e);
				for (auto layerIterator = m_LayerStack.rbegin(); layerIterator != m_LayerStack.rend(); layerIterator++)
				{
					//Sending the events in reverse order here
					layerIterator->get()->OnEvent(e);
				}
				//Clicking to select entities.
				if (e->GetEventType() == Input::EventType::MouseClickEvent)
				{
					auto castEvent = std::static_pointer_cast<Input::MouseClickEvent>(e);
					if (castEvent->GetMouseCode() == ANOR_MOUSE_BUTTON_LEFT && !Input::EventHandler::IsKeyDown(ANOR_KEY_LEFT_CONTROL))
					{
						if (m_ViewportVariables.HoveredPixel != -1)
							m_SceneHierarchyPanel->SetSelectionContext(m_ViewportVariables.HoveredPixel);
					}
				}
			}
		}
	void PushLayer(Ref<Layer> Layer)
		{
			m_LayerStack.pushLayer(Layer);
			Layer->OnAttach();
		}
	void PopLayer()
		{
			//Add OnDetach();
			m_LayerStack.popLayer();
		}
	float DeltaTime()
		{
			float currentFrameRenderTime, deltaTime;
			currentFrameRenderTime = m_OpenGLWindow->GetRenderTime();
			deltaTime = currentFrameRenderTime - m_LastFrameRenderTime;
			m_LastFrameRenderTime = currentFrameRenderTime;
			return deltaTime;
		}
	void OpenScene(const std::filesystem::path& filePath)
		{
			if (!filePath.empty())
			{
				auto& activeScene = m_Layer->GetScene();
				activeScene = std::make_shared<Scene>();
				m_SceneHierarchyPanel->SetContext(m_Layer->GetScene());

				SceneSerializer serializer(activeScene);
				serializer.Deserialize(filePath.string());
				activeScene->OnResizeViewport(m_ViewportVariables.ViewportSize.x, m_ViewportVariables.ViewportSize.y);
			}
		}
	void OpenScene()
		{
			std::string filepath = FileDialogs::OpenFile("Anor Scene (*.anor)\0*.anor\0");
			if (!filepath.empty())
			{
				OpenScene(filepath);
			}
		}
	void CreateScene()
		{
			auto& activeScene = m_Layer->GetScene();
			activeScene = std::make_shared<Scene>();
			m_SceneHierarchyPanel->SetContext(m_Layer->GetScene());
		}
	void SaveSceneAs()
		{
			std::string filepath = FileDialogs::SaveFile("Anor Scene (*.anor)\0*.anor\0");
			if (!filepath.empty())
			{
				SceneSerializer serializer(m_Layer->GetScene());
				serializer.Serialize(filepath);
			}
		}
protected:
	virtual ~AnorEditor()
	{
		WARN("SandboxApp destructor completed!!!");
	}
};
AnorEngine::Application* CreateApplication()
{
	return new AnorEditor("Anor Editor");
}



