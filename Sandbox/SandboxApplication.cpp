#include "Engine.h"
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
		Ref<Shader>shader3;
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
			shader = std::make_shared<Shader>((solutionDir + "AnorEngine\\src\\shaders\\vertex.shader").c_str(), (solutionDir + "AnorEngine\\src\\shaders\\fragment.shader").c_str());
			shader2 = std::make_shared<Shader>((solutionDir + "AnorEngine\\src\\shaders\\vertex.shader").c_str(), (solutionDir + "AnorEngine\\src\\shaders\\lightsourceFrag.shader").c_str());
			shader3 = std::make_shared<Shader>((solutionDir + "AnorEngine\\src\\shaders\\vertex.shader").c_str(), (solutionDir + "AnorEngine\\src\\shaders\\fragment.shader").c_str());
			
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
			
			
			shader3->UploadInteger("tex", 0);
			//shader.UploadFloat3("lightColor", vec3(0.505882f, 0.59609f, 0.70f)); //moonlight color
			shader3->UploadFloat3("lightColor", vec3(1.0f, 1.0f, 1.0f));
			shader3->UploadFloat2("light_pos", vec2(lightX, lightY));
			
			backpack = std::make_shared<Model>((solutionDir + "AnorEngine\\Models\\backpack\\backpack.obj").c_str());
			basketball = std::make_shared<Model>((solutionDir + "AnorEngine\\Models\\ball\\uploads_files_2222080_ball_obj.obj").c_str());
			//Arianna = std::make_shared<Model>((solutionDir + "AnorEngine\\Models\\girl\\Girl_1.obj").c_str());
			
			glEnable(GL_DEPTH_TEST);
			
			basketball->scale(5, 5, 5);
			basketball->translate(0, 0.5f, 0);
			
			//Arianna->scale(5, 5, 5);
			//Arianna->translate(0, 0, -0.7f);
		}
		void OnUpdate() override
		{
			camera->OnUpdate();
			
			backpack->Draw(shader3, camera);
			basketball->Draw(shader3, camera);
			//Arianna->Draw(shader3, camera);

			//renderer3D->singleDraw(light, shader2, camera, 4 * 6);
		}
		void OnEvent() override
		{

		}
	public:
		virtual ~Scene() 
		{	
			WARN("Scene Destructor completed!!");
		}
	};
#endif
	class ExampleLayer : public Layer
	{
		float vertices[21] =
		{
			-3.0f, -1.6f, 0.0f,  0.33f, 0.9f, 0.7f, 1.0f,
			 3.0f, -1.6f, 0.0f,  0.18f, 0.2f, 0.7f, 1.0f,
			 0.0f,  1.6f, 0.0f,  0.69f, 0.01f, 0.7f, 1.0f 
		};
		uint32_t indices[3] =
		{
			0, 1, 2
		};
		Ref<VertexArray> myVAO;
		Ref<Shader> shader; 
		glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
	public:
		glm::vec4 color = { 0,1,1,1 };
	public:
		ExampleLayer()
		{
			myVAO = std::make_shared<VertexArray>();
			std::string solutionDir= __SOLUTION_DIR;
			shader = std::make_shared<Shader>((solutionDir + "AnorEngine\\src\\shaders\\2DVertex.shader").c_str(), (solutionDir + "AnorEngine\\src\\shaders\\2DFragment.shader").c_str());
		}
		void OnAttach() override
		{

			BufferLayout Layout = { {ShaderDataType::vec3, "a_Position", 0} ,{ShaderDataType::vec4, "a_Color", 1} };
			myVAO->AddVertexBuffer(std::make_shared<Buffer>(vertices, 21 * sizeof(float), Layout));
			myVAO->SetIndexBuffer(std::make_shared<IndexBuffer>(indices, 3));
			
		}
		void OnUpdate() override
		{	
			Renderer::Submit(myVAO, shader, m_ModelMatrix, color);
		}
		
	};
	class ExampleLayer2 : public Layer
	{
		float vertices[21] =
		{
			-1.0f, -1.0f, 0.0f, 0.3f, 0.01f, 0.2f,  1.0f,
			 1.0f, -1.0f, 0.0f, 0.3f, 0.18f, 0.15f, 1.0f,
			 0.0f,  1.0f, 0.0f, 0.3f, 0.09f, 0.07f, 1.0f
		};
		uint32_t indices[3] =
		{
			0, 1, 2
		};
		Ref<VertexArray> myVAO;
		Ref<Shader> shader = nullptr;
		glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
	public:
		glm::vec4 color = { 0,1,0,1 };
	public:
		ExampleLayer2()
		{			
			m_ModelMatrix = glm::translate(m_ModelMatrix, { 1,2,0 });
			myVAO = std::make_shared<VertexArray>();
			std::string solutionDir = __SOLUTION_DIR;
			shader = std::make_shared<Shader>((solutionDir + "AnorEngine\\src\\shaders\\2DVertex.shader").c_str(), (solutionDir + "AnorEngine\\src\\shaders\\2DFragment.shader").c_str());
		}
		void OnAttach() override
		{
			BufferLayout Layout = { {ShaderDataType::vec3, "a_Position", 0} ,  {ShaderDataType::vec4, "a_Color", 1} };
			myVAO->AddVertexBuffer(std::make_shared<Buffer>(vertices, 21 * sizeof(float), Layout));
			myVAO->SetIndexBuffer(std::make_shared<IndexBuffer>(indices, 3));
			
		}
		void OnUpdate() override
		{	
			Renderer::Submit(myVAO, shader, m_ModelMatrix, color);
		}
		void OnImGuiRender()
		{
		}
	};


	class Sandbox : public Application
	{
	private:
		LayerStack m_LayerStack; //its a queue really
		Ref<OrthographicCamera> m_OrthoCamera;
		Ref<PerspectiveCamera> m_PersCamera;
		float cameraSpeed = 1;
		glm::vec3 cameraPos = { 0.0f, 0.0f, 0.0f };
		float lastFrame;
		bool isMouseCaptured = false;
		bool isInitialMouseCaptured = false;
		//My Quad Info------------------------------------
		//------------------------------------------------
		float vertices[5 * 4] =
		{
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f
		};
		uint32_t indices[6] =
		{
			0, 1, 2, 2, 3 ,0
		};
		Ref<VertexArray> m_QuadVAO;
		Ref<Shader> m_QuadShader;
		glm::mat4 m_QuadModelMatrix = glm::mat4(1.0f);
		glm::vec3 m_QuadTranslation = { 0.0f, 0.0f, 0.0f };
		glm::vec4 m_QuadColor = { 1,0,0,1 };
		std::string solutionDir = __SOLUTION_DIR;
		Ref<Texture> m_QuadTexture;
		float quadMoveSpeed = 0.1f;
		Ref<ExampleLayer> layer = std::make_shared<ExampleLayer>();
		Ref<ExampleLayer2> layer2 = std::make_shared<ExampleLayer2>();
#ifdef RENDER_MY_SCENE
		Ref<Scene> scene = std::make_shared<Scene>(m_PersCamera);
#endif
		//My Quad Info------------------------------------
		//------------------------------------------------
	public:
		Sandbox()
			:m_OrthoCamera(new OrthographicCamera(-1.6f * 5, 1.6f * 5, -0.9f * 5, 0.9f * 5)), m_PersCamera(new PerspectiveCamera(1920, 1080))
		{	
			Input::EventHandler::SetTargetApplication(this); //Important to set this to the active Application else, you won't get your input processed.
			
			m_QuadVAO = std::make_shared<VertexArray>();
			m_QuadShader = std::make_shared<Shader>((solutionDir + "AnorEngine\\src\\shaders\\2DVertexTexture.shader").c_str(), (solutionDir + "AnorEngine\\src\\shaders\\2DFragmentTexture.shader").c_str());
			m_QuadTexture = std::make_shared<Texture>(solutionDir + "AnorEngine\\textures\\minecraft-diamond.png");
			m_QuadShader->UploadInteger("u_Sampler", 0);
			BufferLayout QuadLayout = { {ShaderDataType::vec3, "a_Position", 0} ,  {ShaderDataType::vec2, "a_TexCoord", 1} };
			m_QuadVAO->AddVertexBuffer(std::make_shared<Buffer>(vertices, 5 * 4 * sizeof(float), QuadLayout));
			m_QuadVAO->SetIndexBuffer(std::make_shared<IndexBuffer>(indices, 6));
#ifdef RENDER_MY_SCENE
			pushLayer(scene);
#endif
			
			pushLayer(layer);
			//pushLayer(layer2);
			//ImGuiEditorLayer->color1 = &m_QuadColor;
			ImGuiEditorLayer->color2 = &layer->color;
			//ImGuiEditorLayer->color3 = &layer2->color;
			pushLayer(ImGuiEditorLayer);
			logInfoDebug();
		}
	protected:
		virtual ~Sandbox() 
		{
			WARN("Sandbox destructor completed!!!");
		}
	public:
		void logInfoDebug() override
		{	
			WARN("APP::{0}", "Custom sandbox application has been created!!");
		}
		void Run() override
		{					
			while (!m_OpenGLWindow->IsClosed())
			{			
				Renderer::BeginScene(m_OrthoCamera);
				Renderer::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
				Renderer::Clear();
				float deltaTime = DeltaTime();
				cameraSpeed =  10 * deltaTime;
				quadMoveSpeed = 3 * deltaTime;
				m_QuadTranslation = {0,0,0};
				ProcessCamera();
				m_OrthoCamera->SetPosition(cameraPos);
				m_QuadModelMatrix = glm::translate(m_QuadModelMatrix, m_QuadTranslation);

				for (Ref<Layer> layer : m_LayerStack)
				{	
					layer->OnUpdate();
				}
				m_QuadTexture->Bind();
				Renderer::Submit(m_QuadVAO, m_QuadShader, m_QuadModelMatrix, m_QuadColor);
				m_QuadTexture->Unbind();

				ImGuiEditorLayer->OnImGuiRender();
				m_OpenGLWindow->Update();
				Renderer::EndScene();
			}
		}
		void pushLayer(Ref<Layer> Layer)
		{
			Layer->OnAttach();
			m_LayerStack.pushLayer(Layer);
		}
		void popLayer()
		{
			m_LayerStack.popLayer();
		}
		void OnEvent(Ref<Input::Event> event) override
		{
			event->Log();
#ifdef RENDER_MY_SCENE
			if (event.get()->GetEventType() == Input::EventType::KeyPressEvent)
			{
				
				if (std::static_pointer_cast<Input::KeyPressEvent>(event)->GetKeyCode() == ANOR_KEY_LEFT_CONTROL) //static cast doesnt do runtime checks to ensure the casted class is a valid class.
				{
					if (!isMouseCaptured)
					{
						m_OpenGLWindow->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
						isMouseCaptured = true;
						ImGuiEditorLayer->wantToCaptureMouse = false;
					}
					else
					{
						m_OpenGLWindow->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
						isMouseCaptured = false;
						isInitialMouseCaptured = false;
						ImGuiEditorLayer->wantToCaptureMouse = true;
					}
				}
			}
			if (event->GetEventType() == Input::EventType::MouseMoveEvent)
			{	
				if (m_OpenGLWindow->GetInputMode(GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
				{
					float xpos = std::static_pointer_cast<Input::MouseMoveEvent>(event)->GetMouseXOffset();
					float ypos = std::static_pointer_cast<Input::MouseMoveEvent>(event)->GetMouseYOffset();

					if (!isInitialMouseCaptured)
					{
						m_PersCamera->lastX = (float)xpos;
						m_PersCamera->lastY = (float)ypos;
						isInitialMouseCaptured = true;
					}
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
			}
#endif
		}
		void ProcessCamera()
		{		
			if (Input::EventHandler::IsKeyDown(ANOR_KEY_W))
			{
#ifdef RENDER_MY_SCENE
				m_PersCamera->cameraPos += cameraSpeed * m_PersCamera->cameraFront;
#else
				cameraPos.y += cameraSpeed;
#endif
			}
			else if (Input::EventHandler::IsKeyDown(ANOR_KEY_S))
			{
#ifdef RENDER_MY_SCENE
				m_PersCamera->cameraPos -= cameraSpeed * m_PersCamera->cameraFront;
#else
				cameraPos.y -= cameraSpeed;
#endif
			}
			if (Input::EventHandler::IsKeyDown(ANOR_KEY_A))
			{
#ifdef RENDER_MY_SCENE
				m_PersCamera->cameraPos -= glm::normalize(glm::cross(m_PersCamera->cameraFront, m_PersCamera->cameraUp)) * cameraSpeed;
#else
				cameraPos.x -= cameraSpeed;
#endif
			}
			else if (Input::EventHandler::IsKeyDown(ANOR_KEY_D))
			{
#ifdef RENDER_MY_SCENE
				m_PersCamera->cameraPos += glm::normalize(glm::cross(m_PersCamera->cameraFront, m_PersCamera->cameraUp)) * cameraSpeed;
#else
				cameraPos.x += cameraSpeed;
#endif
			}
			if (Input::EventHandler::IsKeyDown(ANOR_KEY_I))
				m_QuadTranslation.y += quadMoveSpeed;
			else if(Input::EventHandler::IsKeyDown(ANOR_KEY_K))
				m_QuadTranslation.y -= quadMoveSpeed;
			if(Input::EventHandler::IsKeyDown(ANOR_KEY_J))
				m_QuadTranslation.x -= quadMoveSpeed;
			else if(Input::EventHandler::IsKeyDown(ANOR_KEY_L))
				m_QuadTranslation.x += quadMoveSpeed;
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
		return new Sandbox();
	}
}

