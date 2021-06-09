#include <Core/Engine.h>
#include <Core/EntryPoint.h>
namespace Game
{	
	using namespace AnorEngine;
	using namespace Graphics;
	using namespace Math;
	
	class ExampleLayer : public Layer
	{
		float m_Vertices[21] =
		{
			-3.0f, -1.6f, 0.0f,  0.33f, 0.9f, 0.7f, 0.25f,
			 3.0f, -1.6f, 0.0f,  0.18f, 0.2f, 0.7f, 0.25f,
			 0.0f,  1.6f, 0.0f,  0.69f, 0.01f, 0.7f, 0.25f
		};
		uint32_t m_Indices[3] =
		{
			0, 1, 2
		};
		Ref<VertexArray> m_TriangleVAO;
		Ref<Shader>		 m_TriangleShader; 
		glm::mat4		 m_ModelMatrix = glm::mat4(1.0f);
		glm::vec4		 m_Color = { 0,1,1,1 };
	public:
		ExampleLayer()
		{		
			m_TriangleVAO = std::make_shared<VertexArray>();
			std::string solutionDir= __SOLUTION_DIR;
			m_TriangleShader = ShaderLibrary::GetShader("2DShader");
		}
		virtual void OnAttach() override
		{

			BufferLayout Layout = { {ShaderDataType::vec3, "a_Position", 0} ,{ShaderDataType::vec4, "a_Color", 1} };
			m_TriangleVAO->AddVertexBuffer(std::make_shared<Buffer>(m_Vertices, 21 * sizeof(float), Layout));
			m_TriangleVAO->SetIndexBuffer(std::make_shared<IndexBuffer>(m_Indices, 3));
			
		}
		virtual void OnUpdate(float deltaTime) override
		{	
			Renderer2D::DrawPrimitive(m_TriangleVAO, m_TriangleShader, m_ModelMatrix, m_Color);
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
		float m_Vertices[21] =
		{
			-1.0f, -1.0f, 0.0f, 0.11f, 0.9f, 0.05f,  0.5f,
			 1.0f, -1.0f, 0.0f, 0.3f, 0.2f, 1.0f, 0.5f,
			 0.0f,  1.0f, 0.0f, 0.92f, 0.23f, 0.3f, 0.5f
		};
		uint32_t m_Indices[3] =
		{
			0, 1, 2
		};
		Ref<VertexArray> m_TriangleVAO;
		Ref<Shader>		 m_TriangleShader = nullptr;
		glm::mat4		 m_ModelMatrix = glm::mat4(1.0f);
		glm::vec4		 m_Color = { 0,1,0,1 };
	public:
		ExampleLayer2()
		{			
			m_ModelMatrix = glm::translate(m_ModelMatrix, { 1,2,0 });
			m_TriangleVAO = std::make_shared<VertexArray>();
			m_TriangleShader = ShaderLibrary::GetShader("2DShader");
			std::string solutionDir = __SOLUTION_DIR;
			//shader = ShaderLibrary::LoadShader("2DShader", solutionDir + "AnorEngine\\Assets\\Shaders\\2DShader.shader");
		}
		virtual void OnAttach() override
		{
			BufferLayout Layout = { {ShaderDataType::vec3, "a_Position", 0} ,  {ShaderDataType::vec4, "a_Color", 1} };
			m_TriangleVAO->AddVertexBuffer(std::make_shared<Buffer>(m_Vertices, 21 * sizeof(float), Layout));
			m_TriangleVAO->SetIndexBuffer(std::make_shared<IndexBuffer>(m_Indices, 3));
			
		}
		virtual void OnUpdate(float deltaTime) override
		{	
			Renderer2D::DrawPrimitive(m_TriangleVAO, m_TriangleShader, m_ModelMatrix, m_Color);
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
		float			 m_Vertices[5 * 4] =
		{
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f

			//-1.0f, -1.0f, 0.0f, 0.5f, 1.0f, 0.1f,  0.1f,
			// 1.0f, -1.0f, 0.0f, 0.4f, 1.0f, 0.1f,  0.1f,
			// 1.0f,  1.0f, 0.0f, 0.2f, 1.0f, 0.1f,  0.1f, //example without textures
			//-1.0f,  1.0f, 0.0f, 0.1f, 1.0f, 0.1f,   0.1f
		};
		uint32_t		 m_Indices[6] =
		{
			0, 1, 2, 2, 3, 0
		};
		Ref<VertexArray> m_QuadVAO;
		Ref<Shader>		 m_QuadShader;
		Ref<Texture>	 m_QuadTexture;
		glm::mat4		 m_QuadModelMatrix = glm::mat4(1.0f);
		glm::vec3		 m_QuadTranslation = { 0.0f, 0.0f, 0.0f };
		glm::vec4		 m_QuadColor = { 1,0,0,1 };
		float			 m_QuadMoveSpeed = 5.0f;
		std::string solutionDir = __SOLUTION_DIR;
	public:
		ExampleLayer3()
		{
			m_QuadVAO = std::make_shared<VertexArray>();
			m_QuadShader = ShaderLibrary::GetShader("TextureShader");
			//m_QuadShader = ShaderLibrary::GetShader("2DShader");
			m_QuadTexture = std::make_shared<Texture>(solutionDir + "AnorEngine\\Assets\\Textures\\transparent.png");
			m_QuadShader->UploadInteger("u_Sampler", 0);
		}
		virtual void OnAttach() override
		{
			BufferLayout QuadBufferLayout = { {ShaderDataType::vec3, "a_Position", 0}, { ShaderDataType::vec2, "a_Tex", 1 } };
			m_QuadVAO->AddVertexBuffer(std::make_shared<Buffer>(m_Vertices, 5 * 4 * sizeof(float), QuadBufferLayout));
			m_QuadVAO->SetIndexBuffer(std::make_shared<IndexBuffer>(m_Indices, 6));
		}
		virtual void OnUpdate(float deltaTime) override
		{
			//Querying the EventHandler here so that we can move the quad.
			if (Input::EventHandler::IsKeyDown(ANOR_KEY_I))
				m_QuadTranslation.y += m_QuadMoveSpeed * deltaTime;
			else if (Input::EventHandler::IsKeyDown(ANOR_KEY_K))
				m_QuadTranslation.y -= m_QuadMoveSpeed * deltaTime;
			if (Input::EventHandler::IsKeyDown(ANOR_KEY_J))
				m_QuadTranslation.x -= m_QuadMoveSpeed * deltaTime;
			else if (Input::EventHandler::IsKeyDown(ANOR_KEY_L))
				m_QuadTranslation.x += m_QuadMoveSpeed * deltaTime;

			//Updating quad transform data here
			m_QuadModelMatrix = glm::translate(m_QuadModelMatrix, m_QuadTranslation);
			//Rendering here 
			Renderer2D::DrawPrimitive(m_QuadVAO, m_QuadShader, m_QuadModelMatrix, m_QuadColor, m_QuadTexture);
			//Reset translation after rendering for further use
			m_QuadTranslation = { 0,0,0 }; 
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
			m_BgVAO->AddVertexBuffer(std::make_shared<Buffer>(m_Vertices, 5 * 4 * sizeof(float), Layout));
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
		glm::vec3						  m_CameraPos = { 0.0f, 0.0f, 0.0f };
		float							  m_CameraSpeed = 1;
		float							  lastFrame;
		bool							  m_Minimized = false;
		std::vector<ProfileResult>		  m_ProfileResults;
		//--------------------------------------------------------------------
		std::string solutionDir = __SOLUTION_DIR;
	public:
		SandboxApp2D()
			:m_OrthoCamera(std::make_shared<OrthographicCamera>(-1280.0f / 720.0f * (5), 1280.0f / 720.0f * (5), -1 * (5), 1 * (5))), m_PersCamera(std::make_shared<PerspectiveCamera>(1280, 720))
		{	
			Input::EventHandler::SetTargetApplication(this); //Important to set this to the active Application else, you won't get your input processed.		
			m_ImGuiBase->Init(); // Need to call the initialization code for imgui here.
			m_OrthoGraphicCameraController = std::make_shared<OrthographicCameraController>(m_OrthoCamera, (1280.0f / 720.0f));	
			//Shader Creation-------------------------------------------------------------------------------------------------------------
			ShaderLibrary::LoadShader("2DShader", solutionDir + "AnorEngine\\Assets\\Shaders\\2DShader.shader");
			ShaderLibrary::LoadShader("TextureShader", solutionDir + "AnorEngine\\Assets\\Shaders\\2DTextureShader.shader");
			ShaderLibrary::LoadShader("2DBackgroundShader", solutionDir + "AnorEngine\\Assets\\Shaders\\2DBackgroundShader.shader");
			//Layer Creation--------------------------------------------------------------------------------------------
			m_Layer = std::make_shared<ExampleLayer>();
			m_Layer2 = std::make_shared<ExampleLayer2>();
			m_Layer3 = std::make_shared<ExampleLayer3>();
			m_Bg = std::make_shared<Background>();
			//Partcile Settings----------------------------------------------------------------------------------
			ParticleProperties particleProperties;
			particleProperties.Color = { 1, 1, 1, 0.5f };
			particleProperties.LifeTime = 5.0f;
			particleProperties.MoveDirection = { 0.0f, 0.0f, 0.0f };
			particleProperties.Size = 0.3f;
			particleProperties.Speed = 20.0f;
			m_ParticleSystem = std::make_shared<ParticleSystem>(particleProperties);
			//Layer insertion
			//PushLayer(m_Layer3);
			//PushLayer(m_Layer2);
			//PushLayer(m_Layer);
			PushLayer(m_Bg);
			LogInfoDebug();
		}
	protected:
		virtual ~SandboxApp2D()
		{
			WARN("SandboxApp destructor completed!!!");
		}
	public:
		virtual void Run() override
		{	
			while (!m_OpenGLWindow->IsClosed())
			{
				m_ParticleSystem->CreateParticles(1);
				float deltaTime = DeltaTime();
				{
					PROFILE_SCOPE("BeginScene");
					m_OrthoGraphicCameraController->OnUpdate(deltaTime);			
					Renderer2D::BeginScene(m_OrthoCamera);
					Renderer2D::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
					Renderer2D::Clear();
				}
				{
					PROFILE_SCOPE("OnUpdate");
					if (!m_Minimized) //We don't want to render if the window is minimized.
					{
						for (Ref<Layer> layer : m_LayerStack)
						{	
							layer->OnUpdate(deltaTime);
						}				
						m_ParticleSystem->OnUpdate(deltaTime);
					}
				}
				m_ImGuiBase->Begin();
				for (Ref<Layer> layer : m_LayerStack)
				{
					layer->OnImGuiRender();
				}
				OnImGuiOverlayRender();
				m_ImGuiBase->End();

				m_OpenGLWindow->Update();
				Renderer2D::EndScene();
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
					OnWindowResizeEvent(std::static_pointer_cast<Input::WindowResizeEvent>(e));
				}
				//Passing every single event that I get from the OpenGLWindow to this controller. Will change.
				m_OrthoGraphicCameraController->OnEvent(e);
			}
		}
		virtual void OnWindowResizeEvent(Ref<Input::WindowResizeEvent> e) override
		{
			if (e->GetHeight() == 0 || e->GetWidth() == 0)
				m_Minimized = true;
			else if (e->GetHeight() > 0 || e->GetWidth() > 0)
				m_Minimized = false;
		}
		virtual void LogInfoDebug() override
		{	
			WARN("APP::{0}", "Custom sandbox application has been created!!");
		}
		void OnImGuiOverlayRender()
		{
			static bool show = true;
			for (auto& result : m_ProfileResults)
			{
				char buffer[50];
				strcpy(buffer, "%.3f ms  ");
				strcat(buffer, result.Name);
				ImGui::Text(buffer, result.Time);
			}
			m_ProfileResults.clear();
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
		void ProcessMovementInput() //TODO:Remove
		{	
		}
		float DeltaTime()
		{
			float currentFrame, deltaTime;
			currentFrame = m_OpenGLWindow->GetRenderTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			return deltaTime;
		}
	};
	AnorEngine::Application* CreateApplication()
	{
		return new SandboxApp2D();
	}
}

