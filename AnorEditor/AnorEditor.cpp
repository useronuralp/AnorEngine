#include <Core/Engine.h>
#include <Core/EntryPoint.h>
#include <Panels/SceneHierarchyPanel.h>
#include "Scene/SceneSerializer.h"
#include "Utility/WindowsUtils.h"
namespace Game
{
	using namespace AnorEngine;
	using namespace Graphics;
	using namespace Math;

	class ExampleLayer : public Layer
	{
		bool			    m_IsRuntime = false;
		Ref<EditorCamera>   m_EditorCamera;
		glm::vec4			m_Color = { 1,1,1,1 };
		std::string			solutionDir = __SOLUTION_DIR;
		Ref<Scene>			m_Scene = std::make_shared<Scene>();
		//Entity1----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Ref<Entity>			m_Entity1 = std::make_shared<Entity>(m_Scene->CreateEntity("Diamonds"));
		Ref<Texture>		m_Entity1TextureAtlas = std::make_shared<Texture>(solutionDir + "AnorEngine\\Assets\\Textures\\PlatformerTextures\\Tilesheet\\platformPack_tilesheet@2.png");
		//Entity2----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Ref<Entity>			m_Entity2 = std::make_shared<Entity>(m_Scene->CreateEntity("Block"));
		Ref<Texture>		m_Entity2_Texture = std::make_shared<Texture>(solutionDir + "AnorEngine\\Assets\\Textures\\381f5a63791945.5abc4ccf1297d.png");
		//Entity3----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Ref<Entity>			m_Entity3 = std::make_shared<Entity>(m_Scene->CreateEntity("Player"));
		Ref<Texture>		m_Entity3TextureAtlas = std::make_shared<Texture>(solutionDir + "AnorEngine\\Assets\\Textures\\PlatformerTextures\\Tilesheet\\platformerPack_character@2.png");
		//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Ref<Entity>			m_CameraEntity = std::make_shared<Entity>(m_Scene->CreateEntity("Camera"));
	public:
		Ref<Scene>& GetActiveScene() { return m_Scene; }
		ExampleLayer(Ref<EditorCamera>& camera)
			:m_EditorCamera(camera){}
		virtual void OnAttach() override
		{
			struct CameraController : public ScriptableEntity
			{
				void OnCreate()
				{
				}
				void OnDestroy()
				{
				}
				void OnUpdate(float deltaTime)
				{
					if (Input::EventHandler::IsKeyDown(ANOR_KEY_W))
						GetComponent<TransformComponent>().Translation.y += 10.0f * deltaTime;
					else if (Input::EventHandler::IsKeyDown(ANOR_KEY_S))
						GetComponent<TransformComponent>().Translation.y -= 10.0f * deltaTime;
					if (Input::EventHandler::IsKeyDown(ANOR_KEY_A))
						GetComponent<TransformComponent>().Translation.x -= 10.0f * deltaTime;
					else if (Input::EventHandler::IsKeyDown(ANOR_KEY_D))
						GetComponent<TransformComponent>().Translation.x += 10.0f * deltaTime;
				}
			};
			struct CharacterController : public ScriptableEntity
			{
				void OnCreate()
				{
				}
				void OnDestroy()
				{
				}
				void OnUpdate(float deltaTime)
				{
					if (Input::EventHandler::IsKeyDown(ANOR_KEY_I))
						GetComponent<TransformComponent>().Translation.y += 5.0f * deltaTime;
					else if (Input::EventHandler::IsKeyDown(ANOR_KEY_K))
						GetComponent<TransformComponent>().Translation.y -= 5.0f * deltaTime;
					if (Input::EventHandler::IsKeyDown(ANOR_KEY_J))
						GetComponent<TransformComponent>().Translation.x -= 5.0f * deltaTime;
					else if (Input::EventHandler::IsKeyDown(ANOR_KEY_L))
						GetComponent<TransformComponent>().Translation.x += 5.0f * deltaTime;
				}
			};
			struct EnemyBehaviour : public ScriptableEntity
			{
				void OnCreate()
				{
				}
				void OnDestroy()
				{
				}
				void OnUpdate(float deltaTime)
				{

					auto [playerX, playerY] = GetPlayerLocation();
					auto& position = GetComponent<TransformComponent>().Translation;

					if(abs(playerX - position.x) > 0 && abs(playerY - position.y) > 0)
					{
						position.x += (0.75f * (playerX - position.x)) * deltaTime;
						position.y += (0.75f * (playerY - position.y)) * deltaTime;
					}
				}
			};
			
			if (m_CameraEntity)
			{
				m_CameraEntity->AddComponent<CameraComponent>();
				//You can initialize scripts by passing a bool to their constructors. If it is true the script is enabled upon engine launch. Default value is false.
				m_CameraEntity->AddComponent<NativeScriptComponent>(true).Bind<CameraController>();
			}
			if (m_Entity1)
			{
				m_Entity1->GetComponent<TransformComponent>().Translation.x = 3.5f;
				m_Entity1->GetComponent<TransformComponent>().Translation.y = 3.5f;
				m_Entity1->AddComponent<SpriteRendererComponent>(m_Color, m_Entity1TextureAtlas, glm::vec2{ 2.0f, 1.0f }, glm::vec2{ 9, 3 }, glm::vec2{ 128.0f, 128.0f });
				m_Entity1->AddComponent<NativeScriptComponent>().Bind<EnemyBehaviour>();
			}
			if (m_Entity2)
			{
				m_Entity2->GetComponent<TransformComponent>().Translation.x = -3.5f;
				m_Entity2->GetComponent<TransformComponent>().Translation.y = -3.5f;
				m_Entity2->AddComponent<SpriteRendererComponent>(m_Color, m_Entity2_Texture);
			}
			if (m_Entity3)
			{
				m_Entity3->AddComponent<SpriteRendererComponent>(m_Color, m_Entity3TextureAtlas, glm::vec2{ 1.0f, 1.0f }, glm::vec2{ 0 ,1 }, glm::vec2{ 192.0f, 175.0f });
				m_Entity3->AddComponent<NativeScriptComponent>(true).Bind<CharacterController>();
			}
		}
		virtual void OnUpdate(bool IsRuntime, float deltaTime) override
		{
			IsRuntime ? m_Scene->OnUpdateRuntime(deltaTime) : m_Scene->OnUpdateEditor(deltaTime, m_EditorCamera);
			//m_Scene->OnUpdateEditor(deltaTime, m_EditorCamera);
		}
		virtual void OnEvent(Ref<Input::Event> e) override
		{
			if (!e->m_Handled)
			{		
				////This resizing event handling is currently not needed since the viewport code does the resizing and sends it to the framebuffer.
				////However, I will leave this code here in case the rendering target changes from a framebuffer to a glfw window, then, I would need this resizing event to properly resize the window
				////and if I delete it now, I will have a hard time trying to find the reason why the window won't resize in the future.
				//if (e->GetEventType() == Input::EventType::WindowResizeEvent)
				//{
				//	auto castEvent = std::static_pointer_cast<Input::WindowResizeEvent>(e);
				//	OnResizeViewport(castEvent->GetWidth(), castEvent->GetHeight());
				//	e->m_Handled = true;
				//}
				//if (e->GetEventType() == Input::EventType::MouseScrollEvent)
				//{
				//	auto castEvent = std::static_pointer_cast<Input::MouseScrollEvent>(e);
				//	OnMouseScroll(castEvent->GetXOffset(), castEvent->GetYOffset());
				//	e->m_Handled = true;
				//}
			}
		}
		virtual void OnResizeViewport(uint32_t width, uint32_t height) override
		{
			m_Scene->OnResizeViewport(width, height);
		}
		virtual void OnMouseScroll(float xoffset, float yoffset) override
		{
			m_Scene->OnMouseScroll(xoffset, yoffset);
		}
	};
	class Background : public Layer
	{
	private:
		float m_Vertices[5 * 4] =
		{
			-0.8f, -0.45f, 0.0f, 0.0f, 0.0f,
			 0.8f, -0.45f, 0.0f, 1.0f, 0.0f,
			 0.8f,  0.45f, 0.0f, 1.0f, 1.0f,
			-0.8f,  0.45f, 0.0f, 0.0f, 1.0f
		};
		uint32_t m_Indices[6] =
		{
			0, 1, 2, 2, 3, 0
		};
		Ref<VertexArray> m_BgVAO;
		Ref<Shader>		 m_BgShader = nullptr;
		Ref<Texture>	 m_BgTexture;
		glm::mat4		 m_BgModelMatrix = glm::mat4(1.0f);
		Ref<OrthographicCamera>	     m_MainCamera;
	public:
		Background(Ref<OrthographicCamera> mainCamera)
		{
			m_MainCamera = mainCamera;
			std::string solutionDir = __SOLUTION_DIR;
			m_BgModelMatrix = glm::scale(m_BgModelMatrix, { 12.0f, 12.0f , 1.0f });
			m_BgVAO = std::make_shared<VertexArray>();
			m_BgShader = ShaderLibrary::GetShader("2DBackgroundShader");
			m_BgTexture = std::make_shared<Texture>(solutionDir + "AnorEngine\\Assets\\Textures\\retro.png");
		}
		virtual void OnAttach() override
		{
			BufferLayout Layout = { {ShaderDataType::vec3, "a_Position", 0} ,  {ShaderDataType::vec2, "a_TexCoords", 1} };
			m_BgVAO->AddVertexBuffer(std::make_shared<VertexBuffer>(m_Vertices, 5 * 4 * sizeof(float), Layout));
			m_BgVAO->SetIndexBuffer(std::make_shared<IndexBuffer>(m_Indices, 6));

		}
		virtual void OnUpdate(bool IsRuntime, float deltaTime) override
		{
			//Renderer2D::BeginScene(m_MainCamera);
			//Renderer2D::DrawPrimitive(m_BgVAO, m_BgShader, m_BgModelMatrix, { 1,1,1, 0.4f }, m_BgTexture);
			//Renderer2D::EndScene();
		}
	};
	class AnorEditor : public Application
	{
	private:
		Ref<SceneHierarchyPanel>          m_SceneHierarchyPanel;
		LayerStack						  m_LayerStack;
		Ref<OrthographicCamera>			  m_OrthoCamera;
		Ref<EditorCamera>				  m_EditorCamera;
		Ref<PerspectiveCamera>			  m_PersCamera;
		Ref<ExampleLayer>				  m_Layer;
		Ref<Background>					  m_Bg;
		Ref<ParticleSystem>				  m_ParticleSystem;
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
		std::string					      solutionDir = __SOLUTION_DIR;
	public:
		AnorEditor(const char* appName)
			:Application(appName), m_OrthoCamera(std::make_shared<OrthographicCamera>(-1280.0f / 720.0f * (5), 1280.0f / 720.0f * (5), -1 * (5), 1 * (5))), m_PersCamera(std::make_shared<PerspectiveCamera>(1280, 720))
		{
			m_EditorCamera = std::make_shared<EditorCamera>();
			Input::EventHandler::SetTargetApplication(this); //Important to set this to the active Application else, you won't get your input processed.	
			//Framebuffer Settings --------------------------------------------------------------------------------------------
			FramebufferSpecifications fbSpecs;
			fbSpecs.Attachments = { {FramebufferTextureFormat::RGBA8}, {FramebufferTextureFormat::RED_INTEGER}, {FramebufferTextureFormat::DEPTH24STENCIL8} };
			m_Framebuffer = std::make_shared<Framebuffer>(fbSpecs);
			//Layer Creation--------------------------------------------------------------------------------------------
			m_Layer = std::make_shared<ExampleLayer>(m_EditorCamera);
			m_Bg = std::make_shared<Background>(m_OrthoCamera);
			m_SceneHierarchyPanel = std::make_shared<SceneHierarchyPanel>(m_Layer->GetActiveScene());
			//Particle Settings----------------------------------------------------------------------------------
			ParticleProperties particleProperties;
			particleProperties.Color = { 1, 1, 1, 0.5f };
			particleProperties.LifeTime = 4.0f;
			particleProperties.Size = 0.25f;
			particleProperties.Speed = 2.0f;
			particleProperties.EmissionPoint = { -3.0f, 0.0f,0.0f };
			m_ParticleSystem = std::make_shared<ParticleSystem>(particleProperties);
			//Layer insertion----------------------------------------------------------------------------------------
			PushLayer(m_Layer);
			PushLayer(m_Bg);
		}
	protected:
		virtual ~AnorEditor()
		{
			WARN("SandboxApp destructor completed!!!");
		}
	public:
		virtual void Run() override
		{
			while (!m_OpenGLWindow->IsClosed())
			{
				float deltaTime = DeltaTime();
				m_ParticleSystem->CreateParticles(3);
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
					//Renderer2D::BeginScene(m_OrthoCamera);
					//m_ParticleSystem->OnUpdate(deltaTime);
					//Renderer2D::EndScene();
				}
				m_Framebuffer->Unbind();
				ImGuiBase::Begin(); //-----------------------ImGui Beginning-------------------------

				ImGuiDockspaceSetup();
				m_IsRuntime = m_SceneHierarchyPanel->OnImGuiRender();

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
				ImGui::End();
				ImGui::PopStyleVar();

				ImGuiBase::End(); //-----------------------ImGui END -------------------------------

				//Try to resize the framebuffer at the end. Otherwise it causes weird flickering problems.
				UpdateScreenBoundaries();

				m_EditorCamera->OnUpdate(deltaTime);
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
			// In 99% case you should be able to just call DockSpaceOverViewport() and ignore all the code below!
			// In this specific demo, we are not using DockSpaceOverViewport() because:
			// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
			// - we allow the host window to have padding (when opt_padding == true)
			// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
			// TL;DR; this demo is more complicated than what you would normally use.
			// If we removed all the options we are showcasing, this demo would become:
			//     void ShowExampleAppDockSpace()
			//     {
			//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
			//     }
			static bool dockspaceOpen = true;
			static bool opt_fullscreen = true;
			static bool opt_padding = false;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
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

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
			// and handle the pass-thru hole, so we ask Begin() to not render a background.
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
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
					// Disabling fullscreen would allow the window to be moved to the front of other windows,
					// which we can't undo at the moment without finer window depth/z control.
					if (ImGui::MenuItem("New"))
					{
						auto& activeScene = m_Layer->GetActiveScene();
						activeScene = std::make_shared<Scene>();
						//activeScene->OnResizeViewport(m_ViewportSize.x, m_ViewportSize.y);
						m_SceneHierarchyPanel->SetContext(m_Layer->GetActiveScene());
					}
					if (ImGui::MenuItem("Open..."))
					{
						std::string filepath = FileDialogs::OpenFile("Anor Scene (*.anor)\0*.anor\0");
						if (!filepath.empty())
						{
							auto& activeScene = m_Layer->GetActiveScene();
							activeScene = std::make_shared<Scene>();
							m_SceneHierarchyPanel->SetContext(m_Layer->GetActiveScene());
							SceneSerializer serializer(activeScene);
							serializer.Deserialize(filepath);
							activeScene->OnResizeViewport(m_ViewportSize.x, m_ViewportSize.y);
						}
					}
					if (ImGui::MenuItem("Save As..."))
					{
						std::string filepath = FileDialogs::SaveFile("Anor Scene (*.anor)\0*.anor\0");
						if (!filepath.empty())
						{
							SceneSerializer serializer(m_Layer->GetActiveScene());
							serializer.Serialize(filepath);
						}
					}
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
				//Particle system event
				if (e->GetEventType() == Input::EventType::MouseMoveEvent)
				{
					int width, height;
					m_OpenGLWindow->GetWindowSize(&width, &height);
					float xoffset = width - m_ViewportSize.x;
					float yoffset = height - m_ViewportSize.y;
					float aspectRatio = (float)width / (float)height;
					OrthographicCameraBounds bounds = {-aspectRatio * 5, aspectRatio* 5, -5, 5 };
					m_ParticleSystem->SetEmissionPoint(m_MousePositionRelativeToRenderViewport.x / m_ViewportSize.x * bounds.GetWidth() - bounds.GetWidth() * 0.5f,
						-(bounds.GetHeight() * 0.5f - m_MousePositionRelativeToRenderViewport.y / m_ViewportSize.y * bounds.GetHeight()));
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
	};
	AnorEngine::Application* CreateApplication()
	{
		return new AnorEditor("Anor Editor");
	}
}


