#include "Core/Engine.h"
#include "Core/EntryPoint.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Scene/SceneSerializer.h"
#include "Utility/WindowsUtils.h"
#include "ExampleLayer.h"
#include "Panels/ContentBrowserPanel.h"
namespace AnorEngine
{
	extern const std::filesystem::path g_AssetPath;
	extern const std::filesystem::path g_ResourcesPath;
}
namespace Game
{
	using namespace AnorEngine;
	using namespace Graphics;
	class AnorEditor : public Application
	{
	private:
		Ref<SceneHierarchyPanel>          m_SceneHierarchyPanel;
		Ref<ContentBrowserPanel>          m_ContentBrowserPanel;
		LayerStack						  m_LayerStack;
		Ref<OrthographicCamera>			  m_OrthoCamera;
		Ref<EditorCamera>				  m_EditorCamera;
		Ref<PerspectiveCamera>			  m_PersCamera;
		Ref<ExampleLayer>				  m_Layer;
		Ref<Framebuffer>				  m_Framebuffer;
		glm::vec2						  m_ViewportSize;
		glm::vec2						  m_MousePositionRelativeToRenderViewport;
		glm::vec2						  m_ViewportBounds[2];
		float							  m_LastFrameRenderTime;
		bool							  m_Minimized = false;
		bool							  m_ViewportHovered = false;
		bool							  m_ViewportFocused = false;
		bool							  m_BlockEvents = false;
		bool							  m_IsRuntime = false; 
		int								  m_HoveredPixel = -1;
		std::vector<ProfileResult>		  m_ProfileResults;
	public:
		AnorEditor(const char* appName)
			:Application(appName), m_OrthoCamera(std::make_shared<OrthographicCamera>(-1280.0f / 720.0f * (5), 1280.0f / 720.0f * (5), -1 * (5), 1 * (5))), m_PersCamera(std::make_shared<PerspectiveCamera>(1280, 720))
		{
			Input::EventHandler::SetTargetApplication(this); //Important to set this to the active Application else, you won't get your input processed.	
			//Framebuffer////////////////////////////////////////
			FramebufferSpecifications fbSpecs;
			fbSpecs.Attachments = { {FramebufferTextureFormat::RGBA8}, {FramebufferTextureFormat::RED_INTEGER}, {FramebufferTextureFormat::DEPTH24STENCIL8} };
			m_Framebuffer = std::make_shared<Framebuffer>(fbSpecs);
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
				m_Framebuffer->Bind();
				Renderer2D::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
				Renderer2D::Clear();
				m_Framebuffer->ClearTextureAttachmentWithIntegerValue(1, -1);
				if (!m_Minimized) //We don't want to render if the window is minimized.
				{
					for (Ref<Layer> layer : m_LayerStack)
					{
						layer->OnUpdate(m_IsRuntime, deltaTime);
					}
				}
				m_Framebuffer->Unbind();
				ImGuiBase::Begin(); //-----------------------ImGui Beginning-------------------------

				//ImGui::ShowDemoWindow();
				ImGuiDockspaceSetup();
				m_IsRuntime = m_SceneHierarchyPanel->OnImGuiRender();
				m_ContentBrowserPanel->OnImGuiRender();
				//Frames of the scene will be rendered to this panel. Every frame will be rendered to a framebuffer and ImGui will read that data in here and render it to one of its viewport window objects as a texture. 
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
				ImGui::Begin("Viewport");
				m_ViewportFocused = ImGui::IsWindowFocused();
				m_ViewportHovered = ImGui::IsWindowHovered();
				m_BlockEvents = !m_ViewportFocused || !m_ViewportHovered;
				//Saving the viewport size in m_ViewportSize.
				m_ViewportSize.x = ImGui::GetContentRegionAvail().x;
				m_ViewportSize.y = ImGui::GetContentRegionAvail().y;

				//Mouse picking calculations.
				ReadDataFromMousePos();

				uint32_t texture = m_Framebuffer->GetColorAttachmentID();
				ImGui::Image((void*)texture, { m_Framebuffer->GetDimensions().x, m_Framebuffer->GetDimensions().y }, { 0,1 }, { 1,0 });

				if (ImGui::BeginDragDropTarget())
				{
					
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						if (std::wstring(path).find(L"Scenes\\") != std::wstring::npos)
						{
							OpenScene(g_AssetPath / path);
						}
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::End();
				ImGui::PopStyleVar();

				ImGuiBase::End(); //-----------------------ImGui END -------------------------------

				//Try to resize the framebuffer at the end. Otherwise it causes weird flickering problems.
				UpdateScreenBoundaries();

				m_EditorCamera->OnUpdate(deltaTime, m_ViewportHovered);
				m_ProfileResults.clear();
				m_OpenGLWindow->Update();
			}
		}
		void ReadDataFromMousePos()
		{
			auto viewPortOffset = ImGui::GetCursorPos();
			auto windowSize = ImGui::GetWindowSize();
			auto minBound = ImGui::GetWindowPos();
			ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
			minBound.x += viewPortOffset.x;
			minBound.y += viewPortOffset.y;
			m_ViewportBounds[0] = { minBound.x, minBound.y };
			m_ViewportBounds[1] = { maxBound.x, maxBound.y };
			auto [mx, my] = ImGui::GetMousePos();
			mx -= m_ViewportBounds[0].x;
			my -= m_ViewportBounds[0].y;
			my = m_ViewportSize.y - my;
			int mouseX = (int)mx;
			int mouseY = (int)my;
			m_MousePositionRelativeToRenderViewport.x = mouseX;
			m_MousePositionRelativeToRenderViewport.y = mouseY;
			if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)m_ViewportSize.x && mouseY < (int)m_ViewportSize.y)
			{
				m_Framebuffer->Bind();
				int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
				WARN(pixelData);
				m_HoveredPixel = pixelData;
				m_Framebuffer->Unbind();
			}
		}
		void UpdateScreenBoundaries()
		{
			//Resizing the framebuffer if the ImGui panel happens to get resized. (Framebuffer and the panel sizes should be equal.)
			if (m_Framebuffer->GetDimensions().x != m_ViewportSize.x || m_Framebuffer->GetDimensions().y != m_ViewportSize.y)
			{
				m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
				for (auto layerIterator = m_LayerStack.rbegin(); layerIterator != m_LayerStack.rend(); layerIterator++)
				{
					layerIterator->get()->OnResizeViewport(m_ViewportSize.x, m_ViewportSize.y);
				}
				m_EditorCamera->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			}
		}
		void ImGuiDockspaceSetup()
		{
			static bool dockspaceOpen = true;
			static bool opt_fullscreen = true;
			static bool opt_padding = false;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->WorkPos);
				ImGui::SetNextWindowSize(viewport->WorkSize);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}
			else
			{
				dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
			}
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			if (!opt_padding)
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

			if (!opt_padding)
				ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

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
			// DockSpace
			auto& style = ImGui::GetStyle();
			float minWinSizeX = style.WindowMinSize.x;
			style.WindowMinSize.x = 420.0f;
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}
			style.WindowMinSize.x = minWinSizeX;

			static bool show = true;
			ImGui::End();
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
			if (!m_BlockEvents)
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
					if (castEvent->GetMouseCode() == ANOR_MOUSE_BUTTON_LEFT)
					{
						if (m_HoveredPixel != -1)
							m_SceneHierarchyPanel->SetSelectionContext(m_HoveredPixel);
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
		void OpenScene(const std::filesystem::path& filePath )
		{
			if (!filePath.empty())
			{
				auto& activeScene = m_Layer->GetScene();
				activeScene = std::make_shared<Scene>();
				m_SceneHierarchyPanel->SetContext(m_Layer->GetScene());

				SceneSerializer serializer(activeScene);
				serializer.Deserialize(filePath.string());
				activeScene->OnResizeViewport(m_ViewportSize.x, m_ViewportSize.y);
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
}


