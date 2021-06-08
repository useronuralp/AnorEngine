#include <Core/Engine.h>
#include <Core/EntryPoint.h>
namespace Game
{	
	using namespace AnorEngine;
	using namespace Graphics;
	using namespace Math;
//#define RENDER_MY_SCENE
#ifdef RENDER_MY_SCENE
	class Scene : public Layer
	{	
	private:
		Ref<Shader>shader;
		Ref<Shader>shader2;
		Ref<Renderable3D> light;
		Ref<Model>Arianna;
		Ref<Model>backpack;
		Ref<Model>basketball;
		Ref<Renderer3D>renderer3D;
	public:
		Ref<PerspectiveCamera> camera;
	public:
		Scene (Ref<PerspectiveCamera> camera)
			:Layer("Scene"), camera(camera)
		{
			WARN("Scene default constrcutor that calls 'Layer(const char* name)'");
		}
	public:
		void OnAttach() override
		{	
			std::string solutionDir = __SOLUTION_DIR;
			shader = ShaderLibrary::LoadShader("3DShader", solutionDir + "AnorEngine\\Assets\\Shaders\\3DShader.shader");
			shader2 = ShaderLibrary::LoadShader("3DLightShader", solutionDir + "AnorEngine\\Assets\\Shaders\\3DLightSourceShader.shader");
			
			BufferLayout layout = { {ShaderDataType::vec3, "a_Position", 0} };

			uint32_t totalComponentCount = sizeof(Shape::lightSourceVertx) / sizeof(Shape::lightSourceVertx[0]); // this is how you calculate the element count in a const float* array.
			uint32_t size = totalComponentCount * sizeof(GL_FLOAT); // you can maybe find a way to calculate this in the passed objcect?

			//A Buffer object needs to be always created with a BufferLayout. That is why I currently don't allow a default constructor in there.
			light = std::make_shared<Renderable3D>(std::make_shared<Buffer>(Shape::lightSourceVertx, size ,layout));
			
			float lightX = 2, lightY = 3;
			light->translate(lightX, lightY, 0.0f);
			light->scale(0.3f, 0.3f, 0.3f);
			
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//texture3.Bind(0);
			
			//-----------shader settings------------
			shader->UploadInteger("tex", 0);
			//shader.UploadFloat3("lightColor", vec3(0.505882f, 0.59609f, 0.70f)); //moonlight color
			shader->UploadFloat3("lightColor", vec3(1.0f, 1.0f, 1.0f));
			shader->UploadFloat2("light_pos", vec2(lightX, lightY));
			
			shader2->UploadInteger("tex", 0);
			shader2->UploadFloat4("colour", {1,1,1,1}); //light box 
			//shader2.UploadFloat4("colour", {0.505882f, 0.59609f, 0.70f, 1}); //light box 
			shader2->UploadFloat2("light_pos", vec2(lightX, lightY));
			
			
			backpack = std::make_shared<Model>((solutionDir + "AnorEngine\\Assets\\Models\\backpack\\backpack.obj").c_str());
			basketball = std::make_shared<Model>((solutionDir + "AnorEngine\\Assets\\Models\\ball\\uploads_files_2222080_ball_obj.obj").c_str());
			//Arianna = std::make_shared<Model>((solutionDir + "AnorEngine\\Models\\girl\\Girl_1.obj").c_str());
			
			glEnable(GL_DEPTH_TEST);
			
			basketball->scale(5, 5, 5);
			basketball->translate(0, 0.5f, 0);
			
			//Arianna->scale(5, 5, 5);
			//Arianna->translate(0, 0, -0.7f);
		}
		void OnUpdate(float deltaTime) override
		{
			camera->OnUpdate();
			
			backpack->Draw(shader, camera);
			basketball->Draw(shader, camera);
			//Arianna->Draw(shader, camera);

			//renderer3D->singleDraw(light, shader2, camera, 4 * 6);
		}
	public:
		virtual ~Scene() 
		{	
			WARN("Scene Destructor completed!!");
		}
	};
#endif

	static struct Random
	{
		Random(){ srand(time(NULL));}
		static float GetFloat(){ return static_cast <float> (rand()) / static_cast <float> (RAND_MAX); }
	};
	//Mal erdem 
	//Mal erdem 2
	//Mal erdem 3
	struct Particle
	{
		float m_Vertices[3 * 4] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		uint32_t m_Indices[6] =
		{
			0, 1, 2, 2, 3, 0
		};
		float m_Speed;
		float m_LifeTime;
		float m_Size;
		float m_StartingAlphaValue;
		float m_InitialSpeed;
		bool m_IsDead;
		Ref<VertexArray> m_ParticleVAO;
		Ref<Shader> m_ParticleShader;
		glm::mat4 m_Transform;
		glm::vec3 m_MoveDirection;
		glm::vec4 m_Color;
		std::chrono::steady_clock::time_point m_LifeStartTime;
	public:
		Particle()
		{
			BufferLayout Layout = { {ShaderDataType::vec3, "a_Position", 0} };
			m_LifeStartTime = std::chrono::high_resolution_clock::now();
			m_Speed = 20.0f;
			m_InitialSpeed = m_Speed;
			m_LifeTime = 5;
			m_Size = 1.0f;
			m_MoveDirection = { 0.0f,0.0f,0.0f };
			m_Color = { 1, 1, 1, 0.5f };
			m_IsDead = false;
			m_StartingAlphaValue = m_Color.a;
			m_Transform = glm::mat4(1.0f);
			m_ParticleVAO = std::make_shared<VertexArray>();
			m_ParticleVAO->AddVertexBuffer(std::make_shared<Buffer>(m_Vertices, 12 * sizeof(float), Layout));
			m_ParticleVAO->SetIndexBuffer(std::make_shared<IndexBuffer>(m_Indices, 6));
			m_ParticleShader = ShaderLibrary::GetShader("2DShader");
			//Randomization part----------------------------------------------------------------------------------------
			m_Speed -= Random::GetFloat();
			m_Size -= Random::GetFloat();
			m_Size = std::max(m_Size, 0.05f);
			m_Transform = glm::scale(m_Transform, { m_Size, m_Size, 0 });
			m_MoveDirection.x += Random::GetFloat() * 2.0 - 1.0;
			m_MoveDirection.y += Random::GetFloat() * 2.0 - 1.0;
			m_Color.y -= Random::GetFloat();
		}
	public:
		void OnUpdate(float deltaTime)
		{			
			auto currentTime = std::chrono::high_resolution_clock::now();
			auto elapsedTime = currentTime - m_LifeStartTime;
			if (std::chrono::duration_cast<std::chrono::seconds>(elapsedTime).count() >= m_LifeTime)
				m_IsDead = true;
			m_Color.a -= (m_StartingAlphaValue / m_LifeTime) * deltaTime;
			Renderer2D::DrawPrimitive(m_ParticleVAO, m_ParticleShader, m_Transform, m_Color);
			m_Transform = glm::translate(m_Transform, {m_MoveDirection.x * (m_Speed * deltaTime), m_MoveDirection.y * (m_Speed * deltaTime), 0 });
			m_Speed -= (std::chrono::duration_cast<std::chrono::seconds>(elapsedTime).count() / m_LifeTime) * m_InitialSpeed * deltaTime;
			//m_Transform = glm::rotate(m_Transform, 8.0f * deltaTime, { 0,0,0.5f });

		}
	};


	class ParticleSystem
	{
		glm::vec3 m_StartPosition = { 15.0f, 15.0f , 0.0f};
		std::vector<Ref<Particle>> m_Particles;
	public:
		void CreateParticles(int count)
		{
			for (int i = 0; i < count; i++)
			{
				Ref<Particle> particle = std::make_shared<Particle>();
				particle->m_Transform = glm::translate(particle->m_Transform, m_StartPosition);
				m_Particles.push_back(particle);
			}
		}
		void OnUpdate(float deltaTime)
		{
			for (int i = 0; i < m_Particles.size(); i++)
			{
				if (m_Particles[i]->m_IsDead)
				{
					m_Particles.erase(m_Particles.begin() + i);
					continue;
				}
				m_Particles[i]->OnUpdate(deltaTime);
			}
		}
		void OnImGui()
		{
			float vec2[2] = {0.0f, 0.0f };
			ImGui::SliderFloat2("ParticleSystem position", &vec2[0], 0.0f, 50.0f);
			m_StartPosition.x = vec2[0];
			m_StartPosition.y = vec2[1];
		}
	};


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
		//-----------------
		std::string solutionDir = __SOLUTION_DIR;
#ifdef RENDER_MY_SCENE
		Ref<Scene> scene = std::make_shared<Scene>(m_PersCamera);
#endif
	public:
		SandboxApp2D()
			:m_OrthoCamera(std::make_shared<OrthographicCamera>(-1280.0f / 720.0f * (5), 1280.0f / 720.0f * (5), -1 * (5), 1 * (5))), m_PersCamera(std::make_shared<PerspectiveCamera>(1280, 720))
		{	
			Input::EventHandler::SetTargetApplication(this); //Important to set this to the active Application else, you won't get your input processed.		
			m_ImGuiBase->Init(); // Need to call the initialization code for imgui here.
			m_OrthoGraphicCameraController = std::make_shared<OrthographicCameraController>(m_OrthoCamera, (1280.0f / 720.0f));
#ifdef RENDER_MY_SCENE
			pushLayer(scene);
#endif		
			//Shader Creation
			ShaderLibrary::LoadShader("2DShader", solutionDir + "AnorEngine\\Assets\\Shaders\\2DShader.shader");
			ShaderLibrary::LoadShader("TextureShader", solutionDir + "AnorEngine\\Assets\\Shaders\\2DTextureShader.shader");
			ShaderLibrary::LoadShader("2DBackgroundShader", solutionDir + "AnorEngine\\Assets\\Shaders\\2DBackgroundShader.shader");
			//Layer Creation
			m_Layer = std::make_shared<ExampleLayer>();
			m_Layer2 = std::make_shared<ExampleLayer2>();
			m_Layer3 = std::make_shared<ExampleLayer3>();
			m_Bg = std::make_shared<Background>();
			m_ParticleSystem = std::make_shared<ParticleSystem>();
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
				m_ParticleSystem->OnImGui();
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
#ifdef RENDER_MY_SCENE
			if (e->GetEventType() == Input::EventType::MouseMoveEvent)
			{	
				float xpos = std::static_pointer_cast<Input::MouseMoveEvent>(e)->GetMouseXPosition();
				float ypos = std::static_pointer_cast<Input::MouseMoveEvent>(e)->GetMouseYPosition();


				m_PersCamera->lastX = (float)xpos;
				m_PersCamera->lastY = (float)ypos;

				float xoffset = (float)(xpos - m_PersCamera->lastX);
				float yoffset = (float)(m_PersCamera->lastY - ypos);
				m_PersCamera->lastX = (float)xpos;
				m_PersCamera->lastY = (float)ypos;


				xoffset *= m_PersCamera->sensitivity;
				yoffset *= m_PersCamera->sensitivity;

				m_PersCamera->YAW += xoffset;
				m_PersCamera->PITCH += yoffset;

				if (m_PersCamera->PITCH > 89.0f)
					m_PersCamera->PITCH = 89.0f;
				if (m_PersCamera->PITCH < -89.0f)
					m_PersCamera->PITCH = -89.0f;

				glm::vec3 direction;
				direction.x = cos(glm::radians(m_PersCamera->YAW)) * cos(glm::radians(m_PersCamera->PITCH));
				direction.y = sin(glm::radians(m_PersCamera->PITCH));
				direction.z = sin(glm::radians(m_PersCamera->YAW)) * cos(glm::radians(m_PersCamera->PITCH));
				m_PersCamera->cameraFront = glm::normalize(direction);			
			}
#endif		
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
#ifdef RENDER_MY_SCENE
			//3D Stuff
		if (Input::EventHandler::IsKeyDown(ANOR_KEY_W))
		{
			m_PersCamera->cameraPos += cameraSpeed * m_PersCamera->cameraFront;
		}
		else if (Input::EventHandler::IsKeyDown(ANOR_KEY_S))
		{
			m_PersCamera->cameraPos -= cameraSpeed * m_PersCamera->cameraFront;
		}
		if (Input::EventHandler::IsKeyDown(ANOR_KEY_A))
		{
			m_PersCamera->cameraPos -= glm::normalize(glm::cross(m_PersCamera->cameraFront, m_PersCamera->cameraUp)) * cameraSpeed;
		}
		else if (Input::EventHandler::IsKeyDown(ANOR_KEY_D))
			m_PersCamera->cameraPos += glm::normalize(glm::cross(m_PersCamera->cameraFront, m_PersCamera->cameraUp)) * cameraSpeed;
#endif
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

