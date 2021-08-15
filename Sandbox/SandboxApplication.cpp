#include <Core/Engine.h>
#include <Core/EntryPoint.h>
namespace Game
{	
#ifdef OUTDATED_CODE
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
			Renderer2D::Submit({ 0.0f, 0.0f, 0.0f }, { 2.0f, 1.0f }, m_TextureAtlas, { 9, 3 }, {128.0f, 128.0f});
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

			Renderer2D::Submit(m_QuadPosition, { 1.0f, 1.0f }, m_CharacterTextureAtlas, { 0 ,1 }, {192.0f, 175.0f});
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
			m_BgModelMatrix = glm::scale(m_BgModelMatrix, {12.0f, 12.0f , 1.0f });
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
			Renderer2D::DrawPrimitive(m_BgVAO, m_BgShader, m_BgModelMatrix, {1,1,1, 0.4f}, m_BgTexture);
		}
	};

	class SandboxApp2D : public Application
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
		glm::vec3						  m_CameraPos = { 0.0f, 0.0f, 0.0f };
		float							  m_CameraSpeed = 1;
		float							  m_LastFrameRenderTime;
		bool							  m_Minimized = false;
		bool							  dockspaceOpen  = true;
		std::vector<ProfileResult>		  m_ProfileResults;
		//--------------------------------------------------------------------
		std::string solutionDir = __SOLUTION_DIR;
	public:
		SandboxApp2D()
			:m_OrthoCamera(std::make_shared<OrthographicCamera>(-1280.0f / 720.0f * (5), 1280.0f / 720.0f * (5), -1 * (5), 1 * (5))), m_PersCamera(std::make_shared<PerspectiveCamera>(1280, 720))
		{	
			m_CheckerboardTexture = std::make_shared<Texture>(solutionDir + "AnorEngine\\Assets\\Textures\\checkerboard.jpg");
			Input::EventHandler::SetTargetApplication(this); //Important to set this to the active Application else, you won't get your input processed.		
			m_ImGuiBase->Init(); // Need to call the initialization code for imgui here.
			m_OrthoGraphicCameraController = std::make_shared<OrthographicCameraController>(m_OrthoCamera, (1280.0f / 720.0f));	
			//Layer Creation--------------------------------------------------------------------------------------------
			m_Layer = std::make_shared<ExampleLayer>(); //PROBLEM READING THE DATA
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
			//Layer insertion
			PushLayer(m_Layer3);
			PushLayer(m_Layer2);
			PushLayer(m_Layer);
			PushLayer(m_Bg);
		}
	protected:
		virtual ~SandboxApp2D()
		{
			WARN("SandboxApp destructor completed!!!");
		}
	public:
		virtual void Run() override
		{	
			bool stop = false;
			while (!m_OpenGLWindow->IsClosed())
			{	
				float deltaTime = DeltaTime();
				m_ParticleSystem->CreateParticles(1);
				stop = true;

				m_OrthoGraphicCameraController->OnUpdate(deltaTime);	
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
				}					
				m_ImGuiBase->Begin(); //-----------------------ImGui Beginning-------------------------
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
				ImGui::End();
				m_ImGuiBase->End(); //-----------------------ImGui END -------------------------------

				m_ProfileResults.clear();
				m_OpenGLWindow->Update();			
			}
		}
		virtual void OnEvent(Ref<Input::Event> e) override
		{
			e->Log();	
			//Propogating the recieved event to layers.
			//One of the layers can set the 'm_Handled' value of an event to true so that further propogation is prevented.
			OnImGuiOverlayEvent(e);
			for (auto layerIterator = m_LayerStack.rbegin(); layerIterator != m_LayerStack.rend(); layerIterator++)
			{
				//Sending the events in reverse order here
				layerIterator->get()->OnEvent(e);
			}
			if (!e->m_Handled)
			{
				if (e->GetEventType() == Input::EventType::WindowResizeEvent)
				{
					auto castEvent = std::static_pointer_cast<Input::WindowResizeEvent>(e);
					if (castEvent->GetHeight() == 0 || castEvent->GetWidth() == 0)
						m_Minimized = true;
					else if (castEvent->GetHeight() > 0 || castEvent->GetWidth() > 0)
						m_Minimized = false;
				}
				//Passing every single event that I get from the OpenGLWindow to this controller. Will change.
				m_OrthoGraphicCameraController->OnEvent(e);
			}
			if (e->GetEventType() == Input::EventType::MouseMoveEvent)
			{
				auto ev = std::static_pointer_cast<Input::MouseMoveEvent>(e);
				float x = ev->GetMouseXPosition();
				float y = ev->GetMouseYPosition();
				auto bounds = m_OrthoGraphicCameraController->GetBounds();
				int width, height;
				m_OpenGLWindow->GetWindowSize(&width, &height);
				float aspectRatio = m_OrthoGraphicCameraController->GetAspectRatio();
				m_ParticleSystem->SetEmissionPoint((x) / width * bounds.GetWidth() - bounds.GetWidth() * 0.5f, bounds.GetHeight() * 0.5f - (y) / height* bounds.GetHeight());
			}
		}
		void OnImGuiOverlayEvent(Ref<Input::Event>& e)
		{
			if (ImGui::IsAnyItemHovered())
			{
				//Block events if mouse is hovered over any imgui window.
				e->m_Handled = true;
			}
;		}
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
#endif
	AnorEngine::Application* CreateApplication()
	{
		return nullptr;
	}
}

