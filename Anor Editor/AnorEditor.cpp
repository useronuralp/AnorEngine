#include <Core/Engine.h>
#include <Core/EntryPoint.h>
namespace Game
{
	using namespace AnorEngine;
	using namespace Graphics;
	using namespace Math;

	class ExampleLayer : public Layer
	{
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
		Ref<Entity>			m_Entity3 = std::make_shared<Entity>(m_Scene->CreateEntity("Cute Guy"));
		Ref<Texture>		m_Entity3TextureAtlas = std::make_shared<Texture>(solutionDir + "AnorEngine\\Assets\\Textures\\PlatformerTextures\\Tilesheet\\platformerPack_character@2.png");
		glm::vec3			m_Entity3Position = { 0.0f,0.0f,0.0f };
		float				m_Entity3MoveSpeed = 5.0f;
		//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Ref<Entity>			m_CameraEntity = std::make_shared<Entity>(m_Scene->CreateEntity("Camera"));
	public:
		ExampleLayer()
		{
		}
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
						GetComponent<TransformComponent>().Translate(0, 10.0f * deltaTime, 0);
					else if (Input::EventHandler::IsKeyDown(ANOR_KEY_S))
						GetComponent<TransformComponent>().Translate(0, -10.0f * deltaTime, 0);
					if (Input::EventHandler::IsKeyDown(ANOR_KEY_A))
						GetComponent<TransformComponent>().Translate(10.0f * deltaTime, 0, 0);
					else if (Input::EventHandler::IsKeyDown(ANOR_KEY_D))
						GetComponent<TransformComponent>().Translate(-10.0f * deltaTime, 0, 0);
				}
			};
			if (m_CameraEntity)
			{
				m_CameraEntity->AddComponent<CameraComponent>();
				m_CameraEntity->AddComponent<NativeScriptComponent>().Bind<CameraController>();
			}
			if (m_Entity1)
			{
				m_Entity1->AddComponent<SpriteRendererComponent>(m_Color, m_Entity1TextureAtlas, glm::vec2{ 2.0f, 1.0f }, glm::vec2{ 9, 3 }, glm::vec2{ 128.0f, 128.0f });
			}
			if (m_Entity2)
			{
				m_Entity2->GetComponent<TransformComponent>().Translate(2.0f, 1.0f, 0.0f);
				m_Entity2->AddComponent<SpriteRendererComponent>(m_Color, m_Entity2_Texture);
			}
			if (m_Entity3)
			{
				m_Entity3->GetComponent<TransformComponent>().Translate(1.0f, 1.0f, 0.0f);
				m_Entity3->AddComponent<SpriteRendererComponent>(m_Color, m_Entity3TextureAtlas, glm::vec2{ 1.0f, 1.0f }, glm::vec2{ 0 ,1 }, glm::vec2{ 192.0f, 175.0f });
			}
		}
		virtual void OnUpdate(float deltaTime) override
		{
			
			//Querying the EventHandler here so that we can move the quad.
			if (Input::EventHandler::IsKeyDown(ANOR_KEY_I))
				m_Entity3Position.y += m_Entity3MoveSpeed * deltaTime;
			else if (Input::EventHandler::IsKeyDown(ANOR_KEY_K))
				m_Entity3Position.y -= m_Entity3MoveSpeed * deltaTime;
			if (Input::EventHandler::IsKeyDown(ANOR_KEY_J))
				m_Entity3Position.x -= m_Entity3MoveSpeed * deltaTime;
			else if (Input::EventHandler::IsKeyDown(ANOR_KEY_L))
				m_Entity3Position.x += m_Entity3MoveSpeed * deltaTime;

			m_Entity3->GetComponent<TransformComponent>().Translate(m_Entity3Position.x, m_Entity3Position.y, m_Entity3Position.z);
			m_Entity3Position = { 0.0f, 0.0f, 0.0f };
			m_Scene->OnRender(deltaTime);
		}
		virtual void OnImGuiRender() override
		{
			ImGui::ColorEdit4(&m_Entity1->GetComponent<TagComponent>().Tag[0], glm::value_ptr(m_Entity1->GetComponent<SpriteRendererComponent>().Color));
			ImGui::ColorEdit4(&m_Entity2->GetComponent<TagComponent>().Tag[0], glm::value_ptr(m_Entity2->GetComponent<SpriteRendererComponent>().Color));
			ImGui::ColorEdit4(&m_Entity3->GetComponent<TagComponent>().Tag[0], glm::value_ptr(m_Entity3->GetComponent<SpriteRendererComponent>().Color));
		}
		virtual void OnEvent(Ref<Input::Event> e) override
		{
			if (!e->m_Handled)
			{
				if (e->GetEventType() == Input::EventType::WindowResizeEvent)
				{
					WARN("Resizing window!!!!");
					auto castEvent = std::static_pointer_cast<Input::WindowResizeEvent>(e);
					OnResizeViewport(castEvent->GetWidth(), castEvent->GetHeight());
					e->m_Handled = true;
				}
				if (e->GetEventType() == Input::EventType::MouseScrollEvent)
				{
					WARN("Scrolling!!!!");
					auto castEvent = std::static_pointer_cast<Input::MouseScrollEvent>(e);
					OnMouseScroll(castEvent->GetXOffset(), castEvent->GetYOffset());
					e->m_Handled = true;
				}
			}
		}
	public:
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
		virtual void OnUpdate(float deltaTime) override
		{
			Renderer2D::BeginScene(m_MainCamera);
			Renderer2D::DrawPrimitive(m_BgVAO, m_BgShader, m_BgModelMatrix, { 1,1,1, 0.4f }, m_BgTexture);
			Renderer2D::EndScene();
		}
	};
	class AnorEditor : public Application
	{
	private:
		LayerStack						  m_LayerStack;
		Ref<OrthographicCamera>			  m_OrthoCamera;
		Ref<PerspectiveCamera>			  m_PersCamera;
		Ref<ImGuiBase>					  m_ImGuiBase;
		Ref<ExampleLayer>				  m_Layer;
		Ref<Background>					  m_Bg;
		Ref<ParticleSystem>				  m_ParticleSystem;
		Ref<Framebuffer>				  m_Framebuffer;
		glm::vec2						  m_ViewportSize;
		float							  m_LastFrameRenderTime;
		bool							  m_Minimized = false;
		bool							  m_ViewportHovered = false;
		bool							  m_ViewportFocused = false;
		bool							  m_BlockEvents = false;
		std::vector<ProfileResult>		  m_ProfileResults;
		std::string					      solutionDir = __SOLUTION_DIR;
	public:
		AnorEditor(const char* appName)
			:Application(appName), m_OrthoCamera(std::make_shared<OrthographicCamera>(-1280.0f / 720.0f * (5), 1280.0f / 720.0f * (5), -1 * (5), 1 * (5))), m_PersCamera(std::make_shared<PerspectiveCamera>(1280, 720))
		{
			Input::EventHandler::SetTargetApplication(this); //Important to set this to the active Application else, you won't get your input processed.		
			m_ImGuiBase->Init(); // Need to call the initialization code for imgui here.
			//Framebuffer Settings --------------------------------------------------------------------------------------------
			FramebufferSpecifications FramebufferSpecs;
			FramebufferSpecs.Width = 1280;
			FramebufferSpecs.Height = 720;
			m_Framebuffer = std::make_shared<Framebuffer>(FramebufferSpecs);
			//Layer Creation--------------------------------------------------------------------------------------------
			m_Layer = std::make_shared<ExampleLayer>();
			m_Bg = std::make_shared<Background>(m_OrthoCamera);
			//Particle Settings----------------------------------------------------------------------------------
			ParticleProperties particleProperties;
			particleProperties.Color = { 1, 1, 1, 0.5f };
			particleProperties.LifeTime = 8.0f;
			particleProperties.Size = 0.1f;
			particleProperties.Speed = 3.5f;
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
				if (!m_Minimized) //We don't want to render if the window is minimized.
				{
					for (Ref<Layer> layer : m_LayerStack)
					{
						layer->OnUpdate(deltaTime);
					}
					Renderer2D::BeginScene(m_OrthoCamera);
					m_ParticleSystem->OnUpdate(deltaTime);
					Renderer2D::EndScene();
				}
				m_Framebuffer->Unbind();
				m_ImGuiBase->Begin(); //-----------------------ImGui Beginning-------------------------
				OnImGuiOverlayRender();

				ImGui::Begin("Settings");
				for (Ref<Layer> layer : m_LayerStack)
				{
					layer->OnImGuiRender();
				}
				for (auto& result : m_ProfileResults)
				{
					char buffer[50];
					strcpy(buffer, "%.3f ms  ");
					strcat(buffer, result.Name);
					ImGui::Text(buffer, result.Time);
				}
				ImGui::Text("Quad Indices Count: %d", Renderer2D::GetIndexCount());
				ImGui::Text("Quad Count: %d", Renderer2D::GetIndexCount() / 6);
				ImGui::Text("Number of Draw Calls: %d", Renderer2D::GetNumberOfDrawCalls());
				ImGui::Text("Viewport Size: %.0lf, %.0lf", m_Framebuffer->GetDimensions().x, m_Framebuffer->GetDimensions().y);
				ImGui::End();

				//Frames of the scene will be rendered to this panel. Every frame will be rendered to a framebuffer and ImGui will read that data in here and render it to one of its viewport window objects as a texture. 
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
				ImGui::Begin("Viewport");
				m_ViewportFocused = ImGui::IsWindowFocused();
				m_ViewportHovered = ImGui::IsWindowHovered();
				m_BlockEvents = !m_ViewportFocused || !m_ViewportHovered;
				//Saving the viewport size in m_ViewportSize.
				m_ViewportSize.x = ImGui::GetContentRegionAvail().x;
				m_ViewportSize.y = ImGui::GetContentRegionAvail().y;
				uint32_t texture = m_Framebuffer->GetColorAttachmentID();
				ImGui::Image((void*)texture, { m_Framebuffer->GetDimensions().x, m_Framebuffer->GetDimensions().y }, { 0,1 }, { 1,0 });
				ImGui::End();
				ImGui::PopStyleVar();
				m_ImGuiBase->End(); //-----------------------ImGui END -------------------------------

				//Resizing the framebuffer if the ImGui panel happens to get resized. (Framebuffer and the panel sizes should be equal.)
				//Try to resize the framebuffer at the end. Otherwise it causes weird flickering problems.
				if (m_Framebuffer->GetDimensions().x != m_ViewportSize.x || m_Framebuffer->GetDimensions().y != m_ViewportSize.y)
				{
					m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
					for (auto layerIterator = m_LayerStack.rbegin(); layerIterator != m_LayerStack.rend(); layerIterator++)
					{
						layerIterator->get()->OnResizeViewport(m_ViewportSize.x, m_ViewportSize.y);
					}
				}
				m_ProfileResults.clear();
				m_OpenGLWindow->Update();
			}
		}
		virtual void OnEvent(Ref<Input::Event> e) override
		{
			e->Log();
			//Propogating the recieved event to layers.
			//One of the layers can set the 'm_Handled' value of an event to true so that further propogation is prevented.
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
				for (auto layerIterator = m_LayerStack.rbegin(); layerIterator != m_LayerStack.rend(); layerIterator++)
				{
					//Sending the events in reverse order here
					layerIterator->get()->OnEvent(e);
				}
				if (e->GetEventType() == Input::EventType::MouseMoveEvent)
				{
					auto ev = std::static_pointer_cast<Input::MouseMoveEvent>(e);
					float x = ev->GetMouseXPosition();
					float y = ev->GetMouseYPosition();
					int width, height;
					m_OpenGLWindow->GetWindowSize(&width, &height);
					float xoffset = width - m_ViewportSize.x;
					float yoffset = height - m_ViewportSize.y;
					float aspectRatio = (float)width / (float)height;
					OrthographicCameraBounds bounds = {-aspectRatio * 5, aspectRatio* 5, -5, 5 };
					m_ParticleSystem->SetEmissionPoint((x - xoffset) / m_ViewportSize.x * bounds.GetWidth() - bounds.GetWidth() * 0.5f, bounds.GetHeight() * 0.5f - (y - yoffset) / m_ViewportSize.y * bounds.GetHeight());
				}
			}
		}
		void OnImGuiOverlayRender()
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

			// DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Options"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows,
					// which we can't undo at the moment without finer window depth/z control.
					ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
					ImGui::MenuItem("Padding", NULL, &opt_padding);
					ImGui::Separator();

					if (ImGui::MenuItem("Exit")) { /*Application::Get().Close();*/ }

					ImGui::Separator();

					if (ImGui::MenuItem("Close", NULL, false, dockspaceOpen != NULL))
						//dockspaceOpen = false;
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}
			static bool show = true;
			ImGui::End();
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


