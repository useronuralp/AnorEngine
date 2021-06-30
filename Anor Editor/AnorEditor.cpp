#include <Core/Engine.h>
#include <Core/EntryPoint.h>
namespace Game
{
	using namespace AnorEngine;
	using namespace Graphics;
	using namespace Math;

	class ExampleLayer : public Layer
	{
		glm::vec4 m_Color = { 0,1,1,1 };
		std::string solutionDir = __SOLUTION_DIR;
		Ref<Texture> m_TextureAtlas = std::make_shared<Texture>(solutionDir + "AnorEngine\\Assets\\Textures\\PlatformerTextures\\Tilesheet\\platformPack_tilesheet@2.png");;
	public:
		virtual void OnUpdate(float deltaTime) override
		{
			Renderer2D::Submit({ 0.0f, 0.0f, 0.0f }, { 2.0f, 1.0f }, m_TextureAtlas, { 9, 3 }, { 128.0f, 128.0f });
		}
		virtual void OnImGuiRender() override
		{
			ImGui::ColorEdit4("Layer 1", glm::value_ptr(m_Color));
		}
		virtual void OnEvent(Ref<Input::Event> e) override
		{
			//Handle event if it was not handled before by a layer that resides higher in the LayerStack.
			if (!e->m_Handled)
			{
				//Reminder: You can set the m_Handled to true. If you want to block the propogation of this event.
			}
		}
	};
	class ExampleLayer2 : public Layer
	{
	private:
		glm::vec4 m_Color = { 0,1,0,1 };
		std::string solutionDir = __SOLUTION_DIR;
		Ref<Texture> m_Texture = std::make_shared<Texture>(solutionDir + "AnorEngine\\Assets\\Textures\\381f5a63791945.5abc4ccf1297d.png");
	public:
		virtual void OnUpdate(float deltaTime) override
		{
			Renderer2D::Submit({ 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f }, m_Texture);
		}
		virtual void OnImGuiRender() override
		{
			ImGui::ColorEdit4("Layer 2", glm::value_ptr(m_Color));
		}
		virtual void OnEvent(Ref<Input::Event> e) override
		{
			//Handle event if it was not handled before by a layer that resides higher in the LayerStack.
			if (!e->m_Handled)
			{
				//Reminder: You can set the m_Handled to true. If you want to block the propogation of this event.
			}
		}
	};
	class ExampleLayer3 : public Layer
	{
	private:
		glm::vec4		 m_QuadColor = { 1,0,0,1 };
		glm::vec3		 m_QuadPosition = { 2.0f, 3.0f, 0.0f };
		float			 m_QuadMoveSpeed = 5.0f;
		std::string solutionDir = __SOLUTION_DIR;
		Ref<Texture> m_CharacterTextureAtlas = std::make_shared<Texture>(solutionDir + "AnorEngine\\Assets\\Textures\\PlatformerTextures\\Tilesheet\\platformerPack_character@2.png");;
	public:
		virtual void OnUpdate(float deltaTime) override
		{
			//Querying the EventHandler here so that we can move the quad.
			if (Input::EventHandler::IsKeyDown(ANOR_KEY_I))
				m_QuadPosition.y += m_QuadMoveSpeed * deltaTime;
			else if (Input::EventHandler::IsKeyDown(ANOR_KEY_K))
				m_QuadPosition.y -= m_QuadMoveSpeed * deltaTime;
			if (Input::EventHandler::IsKeyDown(ANOR_KEY_J))
				m_QuadPosition.x -= m_QuadMoveSpeed * deltaTime;
			else if (Input::EventHandler::IsKeyDown(ANOR_KEY_L))
				m_QuadPosition.x += m_QuadMoveSpeed * deltaTime;

			Renderer2D::Submit(m_QuadPosition, { 1.0f, 1.0f }, m_CharacterTextureAtlas, { 0 ,1 }, { 192.0f, 175.0f });
		}
		virtual void OnImGuiRender() override
		{
			ImGui::ColorEdit4("Layer 3", glm::value_ptr(m_QuadColor));
		}
		virtual void OnEvent(Ref<Input::Event> e) override
		{
			//Handle the event if it was not handled before by a layer that resides higher in the LayerStack.
			if (!e->m_Handled)
			{
				//Reminder: You can set the m_Handled to true. If you want to block the propogation of this event.
			}
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
	public:
		Background()
		{
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
			Renderer2D::DrawPrimitive(m_BgVAO, m_BgShader, m_BgModelMatrix, { 1,1,1, 0.4f }, m_BgTexture);
		}
	};

	class AnorEditor : public Application
	{
	private:
		LayerStack						  m_LayerStack;
		Ref<OrthographicCamera>			  m_OrthoCamera;
		Ref<OrthographicCameraController> m_OrthoGraphicCameraController;
		Ref<PerspectiveCamera>			  m_PersCamera;
		Ref<ImGuiBase>					  m_ImGuiBase = std::make_shared<ImGuiBase>(); //ImGui initializiton code.
		Ref<ExampleLayer>				  m_Layer;
		Ref<ExampleLayer2>				  m_Layer2;
		Ref<ExampleLayer3>				  m_Layer3;
		Ref<Background>					  m_Bg;
		Ref<ParticleSystem>				  m_ParticleSystem;
		Ref<Texture>					  m_CheckerboardTexture;
		Ref<Framebuffer>				  m_Framebuffer;
		glm::vec2						  m_ViewportSize;
		glm::vec3						  m_CameraPos = { 0.0f, 0.0f, 0.0f };
		float							  m_CameraSpeed = 1;
		float							  m_LastFrameRenderTime;
		bool							  m_Minimized = false;
		std::vector<ProfileResult>		  m_ProfileResults;
		//--------------------------------------------------------------------
		std::string solutionDir = __SOLUTION_DIR;
	public:
		AnorEditor(const char* appName)
			:Application(appName), m_OrthoCamera(std::make_shared<OrthographicCamera>(-1280.0f / 720.0f * (5), 1280.0f / 720.0f * (5), -1 * (5), 1 * (5))), m_PersCamera(std::make_shared<PerspectiveCamera>(1280, 720))
		{
			m_CheckerboardTexture = std::make_shared<Texture>(solutionDir + "AnorEngine\\Assets\\Textures\\checkerboard.jpg");
			Input::EventHandler::SetTargetApplication(this); //Important to set this to the active Application else, you won't get your input processed.		
			m_ImGuiBase->Init(); // Need to call the initialization code for imgui here.
			m_OrthoGraphicCameraController = std::make_shared<OrthographicCameraController>(m_OrthoCamera, (1280.0f / 720.0f));
			//Framebuffer Settings --------------------------------------------------------------------------------------------
			FramebufferSpecifications FramebufferSpecs;
			FramebufferSpecs.Width = 1280;
			FramebufferSpecs.Height = 720;
			m_Framebuffer = std::make_shared<Framebuffer>(FramebufferSpecs);
			//Layer Creation--------------------------------------------------------------------------------------------
			m_Layer = std::make_shared<ExampleLayer>();
			m_Layer2 = std::make_shared<ExampleLayer2>();
			m_Layer3 = std::make_shared<ExampleLayer3>();
			m_Bg = std::make_shared<Background>();
			//Particle Settings----------------------------------------------------------------------------------
			ParticleProperties particleProperties;
			particleProperties.Color = { 1, 1, 1, 0.5f };
			particleProperties.LifeTime = 8.0f;
			particleProperties.Size = 0.3f;
			particleProperties.Speed = 10.0f;
			particleProperties.EmissionPoint = { -3.0f, 0.0f,0.0f };
			m_ParticleSystem = std::make_shared<ParticleSystem>(particleProperties);
			//Layer insertion----------------------------------------------------------------------------------------
			PushLayer(m_Layer3);
			PushLayer(m_Layer2);
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
				m_ParticleSystem->CreateParticles(1);
				m_OrthoGraphicCameraController->OnUpdate(deltaTime);

				m_Framebuffer->Bind();
				Renderer2D::BeginScene(m_OrthoCamera);
				Renderer2D::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
				Renderer2D::Clear();
				if (!m_Minimized) //We don't want to render if the window is minimized.
				{
					{
						PROFILE_SCOPE("Layers' draw calls");
						for (Ref<Layer> layer : m_LayerStack)
						{
							layer->OnUpdate(deltaTime);
						}
					}
					{
						PROFILE_SCOPE("Particle System Batching");
						m_ParticleSystem->OnUpdate(deltaTime);
					}
				}
				{
					PROFILE_SCOPE("Flushing");
					Renderer2D::EndScene();
					m_Framebuffer->Unbind();
				}
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

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
				ImGui::Begin("Viewport");
				m_ViewportSize.x = ImGui::GetContentRegionAvail().x;
				m_ViewportSize.y = ImGui::GetContentRegionAvail().y;
				uint32_t texture = m_Framebuffer->GetColorAttachmentID();
				ImGui::Image((void*)texture, { m_Framebuffer->GetDimensions().x, m_Framebuffer->GetDimensions().y }, { 0,1 }, { 1,0 });
				ImGui::End();
				ImGui::PopStyleVar();
				m_ImGuiBase->End(); //-----------------------ImGui END -------------------------------

				//Setting up the framebuffer for the next frame.
				if (m_Framebuffer->GetDimensions().x != m_ViewportSize.x || m_Framebuffer->GetDimensions().y != m_ViewportSize.y)
				{
					m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
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
			if (!e->m_Handled)
			{
				for (auto layerIterator = m_LayerStack.rbegin(); layerIterator != m_LayerStack.rend(); layerIterator++)
				{
					//Sending the events in reverse order here
					layerIterator->get()->OnEvent(e);
				}
				if (e->GetEventType() == Input::EventType::WindowResizeEvent)
				{
					auto castEvent = std::static_pointer_cast<Input::WindowResizeEvent>(e);
					if (castEvent->GetHeight() == 0 || castEvent->GetWidth() == 0)
						m_Minimized = true;
					else if (castEvent->GetHeight() > 0 || castEvent->GetWidth() > 0)
						m_Minimized = false;
				}
				m_OrthoGraphicCameraController->OnEvent(e);
				if (e->GetEventType() == Input::EventType::MouseMoveEvent)
				{
					auto ev = std::static_pointer_cast<Input::MouseMoveEvent>(e);
					m_ParticleSystem->SetEmissionPoint(((ev->GetMouseXPosition()) / 1280.0f - 0.5f) * m_OrthoGraphicCameraController->GetAspectRatio() / 0.45f, -((ev->GetMouseYPosition()) / 720.0f - 0.5f) * m_OrthoGraphicCameraController->GetAspectRatio() / 0.8f);
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
		void ProcessMovementInput() //TODO:Remove
		{
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


