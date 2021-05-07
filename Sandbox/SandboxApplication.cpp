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
		Shader* shader = nullptr, * shader2 = nullptr, * shader3 = nullptr;
		Renderable3D *light = nullptr;
		Model *Arianna = nullptr, *backpack = nullptr, *basketball = nullptr;
		Renderer3D renderer3D;
		PerspectiveCamera* camera;
	public:
		Scene (PerspectiveCamera* camera)
			:Layer("Scene"), camera(camera)
		{
			WARN("Scene default constrcutor that calls 'Layer(const char* name)'");
		}
	public:
		void OnAttach() override
		{	
			std::string solutionDir = __SOLUTION_DIR;
			shader = new Shader((solutionDir + "AnorEngine\\src\\shaders\\vertex.shader").c_str(), (solutionDir + "AnorEngine\\src\\shaders\\fragment.shader").c_str());
			shader2 = new Shader((solutionDir + "AnorEngine\\src\\shaders\\vertex.shader").c_str(), (solutionDir + "AnorEngine\\src\\shaders\\lightsourceFrag.shader").c_str());
			shader3 = new Shader((solutionDir + "AnorEngine\\src\\shaders\\vertex.shader").c_str(), (solutionDir + "AnorEngine\\src\\shaders\\fragment.shader").c_str());
			
			BufferLayout layout = { {ShaderDataType::vec3, "a_Position", 0} };

			uint32_t totalComponentCount = sizeof(Shape::lightSourceVertx) / sizeof(Shape::lightSourceVertx[0]); // this is how you calculate the element count in a const float* array.
			uint32_t size = totalComponentCount * sizeof(GL_FLOAT); // you can maybe find a way to calculate this in the passed objcect?

			//A Buffer object needs to be always created with a BufferLayout. That is why I currently don't allow a default constructor in there.
			light = new Renderable3D(std::make_shared<Buffer>(Shape::lightSourceVertx, size ,layout));
			
			float lightX = 2, lightY = 3;
			light->translate(lightX, lightY, 0.0f);
			light->scale(0.3f, 0.3f, 0.3f);
			
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//texture3.Bind(0);
			
			//-----------shader settings------------
			shader->setUniform1i("tex", 0);
			//shader.setUniform3f("lightColor", vec3(0.505882f, 0.59609f, 0.70f)); //moonlight color
			shader->setUniform3f("lightColor", vec3(1.0f, 1.0f, 1.0f));
			shader->setUniform2f("light_pos", vec2(lightX, lightY));
			
			shader2->setUniform1i("tex", 0);
			shader2->setUniform4f("colour", vec4(1, 1, 1, 1)); //light box 
			//shader2.setUniform4f("colour", vec4(0.505882f, 0.59609f, 0.70f, 1)); //light box 
			shader2->setUniform2f("light_pos", vec2(lightX, lightY));
			
			
			shader3->setUniform1i("tex", 0);
			//shader.setUniform3f("lightColor", vec3(0.505882f, 0.59609f, 0.70f)); //moonlight color
			shader3->setUniform3f("lightColor", vec3(1.0f, 1.0f, 1.0f));
			shader3->setUniform2f("light_pos", vec2(lightX, lightY));
			
			backpack = new Model((solutionDir + "AnorEngine\\Models\\backpack\\backpack.obj").c_str());
			basketball = new Model((solutionDir + "AnorEngine\\Models\\ball\\uploads_files_2222080_ball_obj.obj").c_str());
			//Arianna = new Model((solutionDir +"AnorEngine\\Models\\girl\\Girl_1.obj").c_str());
			
			glEnable(GL_DEPTH_TEST);
			
			basketball->scale(5, 5, 5);
			basketball->translate(0, 0.5f, 0);
			
			//Arianna->scale(5, 5, 5);
			//Arianna->translate(0, 0, -0.7f);
		}
		void OnUpdate() override
		{
			camera->OnUpdate();
			
			backpack->Draw(*shader3, camera);
			basketball->Draw(*shader3, camera);
			//Arianna->Draw(*shader3, camera);

			renderer3D.singleDraw(*light, *shader2, camera, 4 * 6);
		}
		void OnEvent() override
		{

		}
	public:
		virtual ~Scene() 
		{	
			delete shader;
			delete shader2;
			delete shader3;
			delete light;
			delete Arianna;
			delete backpack;
			delete basketball;
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
		std::shared_ptr<VertexArray> myVAO;
		Shader* shader = nullptr; 
	public:
		ExampleLayer()
		{
			myVAO = std::make_shared<VertexArray>();
			std::string solutionDir= __SOLUTION_DIR;
			shader = new Shader((solutionDir + "AnorEngine\\src\\shaders\\2DVertex.shader").c_str(), (solutionDir + "AnorEngine\\src\\shaders\\2DFragment.shader").c_str());
		}
		void OnAttach() override
		{
			BufferLayout Layout = { {ShaderDataType::vec3, "a_Position", 0} ,{ShaderDataType::vec4, "a_Color", 1} };
			myVAO->AddVertexBuffer(std::make_shared<Buffer>(vertices, 21 * sizeof(float), Layout));
			myVAO->SetIndexBuffer(std::make_shared<IndexBuffer>(indices, 3));
			
		}
		void OnUpdate() override
		{	
			Renderer::Submit(myVAO, shader);
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
		std::shared_ptr<VertexArray> myVAO;
		Shader* shader = nullptr;
	public:
		ExampleLayer2()
		{
			myVAO = std::make_shared<VertexArray>();
			std::string solutionDir = __SOLUTION_DIR;
			shader = new Shader((solutionDir + "AnorEngine\\src\\shaders\\2DVertex.shader").c_str(), (solutionDir + "AnorEngine\\src\\shaders\\2DFragment.shader").c_str());
		}
		void OnAttach() override
		{
			BufferLayout Layout = { {ShaderDataType::vec3, "a_Position", 0} ,  {ShaderDataType::vec4, "a_Color", 1} };
			myVAO->AddVertexBuffer(std::make_shared<Buffer>(vertices, 21 * sizeof(float), Layout));
			myVAO->SetIndexBuffer(std::make_shared<IndexBuffer>(indices, 3));

		}
		void OnUpdate() override
		{
			Renderer::Submit(myVAO, shader);
		}

	};


	class Sandbox : public Application
	{
	private:
		LayerStack m_LayerStack; //its a queue really
		OrthographicCamera* m_OrthoCamera;
		PerspectiveCamera* m_PersCamera;
		float cameraSpeed = 1;
		glm::vec3 cameraPos = { 0.0f, 0.0f, 0.0f };
		float lastFrame;
		bool isMouseCaptured = false;
		bool isInitialMouseCaptured = false;
		bool ImGuiWantToCaptureMouse = true;
	public:
		Sandbox()
			:m_OrthoCamera(new OrthographicCamera(-1.6f * 5, 1.6f * 5, -0.9f * 5, 0.9f * 5)), m_PersCamera(new PerspectiveCamera(1920, 1080))
		{	
			Input::EventHandler::SetTargetApplication(this); //Important to set this to the active Application else, you won't get your input processed.		
#ifdef RENDER_MY_SCENE
			pushLayer(new Scene(m_PersCamera));
#endif
			pushLayer(new ExampleLayer());
			pushLayer(new ExampleLayer2());
			pushLayer(ImGui); //Application class now has its own ImGui pointer which is a lot better than storing it inside OpenGLWindow class.
			logInfoDebug();
		}
	protected:
		virtual ~Sandbox() 
		{
			for (auto layer = m_LayerStack.end(); layer != m_LayerStack.begin();)
			{	
				delete (*--layer); //Look in to the order of destructor calls and virutal keyword.
			}
			WARN("Sandbox destructor completed!!!");
		}
	public:
		void logInfoDebug() override
		{	
			WARN("APP::{0}", "Custom sandbox application has been created!!");
		}
		void Run() override
		{			
			//cam->SetPosition(glm::vec3(2.5f, 3.0f, 0));
			//cam->SetRotation(90.0f);		
			while (!m_OpenGLWindow->IsClosed())
			{			
				Renderer::BeginScene(m_OrthoCamera);
				Renderer::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
				Renderer::Clear();
				cameraSpeed =  10 * DeltaTime();
				ProcessCamera();
				m_OrthoCamera->SetPosition(cameraPos);

				for (Layer* layer : m_LayerStack)
				{	
					layer->OnUpdate();
				}
				m_OpenGLWindow->Update();
				Renderer::EndScene();
			}
			delete m_OpenGLWindow;
		}
		void pushLayer(Layer *Layer)
		{
			Layer->OnAttach();
			m_LayerStack.pushLayer(*Layer);
		}
		void popLayer()
		{
			m_LayerStack.popLayer();
		}
		void OnEvent(Input::Event* event) override
		{
			event->Log();
#ifdef RENDER_MY_SCENE
			if (event->GetEventType() == Input::EventType::KeyPressEvent)
			{
				if (dynamic_cast<Input::KeyPressEvent*>(event)->GetKeyCode() == ONURALP_KEY_LEFT_CONTROL)
				{
					if (!isMouseCaptured)
					{
						m_OpenGLWindow->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
						isMouseCaptured = true;
						ImGuiWantToCaptureMouse = false;
					}
					else
					{
						m_OpenGLWindow->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
						isMouseCaptured = false;
						ImGuiWantToCaptureMouse = true;
						isInitialMouseCaptured = false;
					}
				}
			}
			if (event->GetEventType() == Input::EventType::MouseMoveEvent)
			{	
				if (m_OpenGLWindow->GetInputMode(GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
				{
					float xpos = dynamic_cast<Input::MouseMoveEvent*>(event)->GetMouseXOffset();
					float ypos = dynamic_cast<Input::MouseMoveEvent*>(event)->GetMouseYOffset();

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
			delete event;
		}
		void ProcessCamera()
		{		
			if (Input::EventHandler::IsKeyDown(ONURALP_KEY_W))
			{
#ifdef RENDER_MY_SCENE
				m_PersCamera->cameraPos += cameraSpeed * m_PersCamera->cameraFront;
#else
				cameraPos.y += cameraSpeed;
#endif
			}
			else if (Input::EventHandler::IsKeyDown(ONURALP_KEY_S))
			{
#ifdef RENDER_MY_SCENE
				m_PersCamera->cameraPos -= cameraSpeed * m_PersCamera->cameraFront;
#else
				cameraPos.y -= cameraSpeed;
#endif
			}
			if (Input::EventHandler::IsKeyDown(ONURALP_KEY_A))
			{
#ifdef RENDER_MY_SCENE
				m_PersCamera->cameraPos -= glm::normalize(glm::cross(m_PersCamera->cameraFront, m_PersCamera->cameraUp)) * cameraSpeed;
#else
				cameraPos.x -= cameraSpeed;
#endif
			}
			else if (Input::EventHandler::IsKeyDown(ONURALP_KEY_D))
			{
#ifdef RENDER_MY_SCENE
				m_PersCamera->cameraPos += glm::normalize(glm::cross(m_PersCamera->cameraFront, m_PersCamera->cameraUp)) * cameraSpeed;
#else
				cameraPos.x += cameraSpeed;
#endif
			}
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

