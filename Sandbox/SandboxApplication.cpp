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
		Ref<Shader> m_TriangleShader; 
		glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
		glm::vec4 m_Color = { 0,1,1,1 };
	public:
		ExampleLayer()
		{
			m_TriangleVAO = std::make_shared<VertexArray>();
			std::string solutionDir= __SOLUTION_DIR;
			m_TriangleShader = ShaderLibrary::LoadShader("2DShader", solutionDir + "AnorEngine\\Assets\\Shaders\\2DShader.shader");
		}
		virtual void OnAttach() override
		{

			BufferLayout Layout = { {ShaderDataType::vec3, "a_Position", 0} ,{ShaderDataType::vec4, "a_Color", 1} };
			m_TriangleVAO->AddVertexBuffer(std::make_shared<Buffer>(m_Vertices, 21 * sizeof(float), Layout));
			m_TriangleVAO->SetIndexBuffer(std::make_shared<IndexBuffer>(m_Indices, 3));
			
		}
		virtual void OnUpdate(float deltaTime) override
		{	
			Renderer::Submit(m_TriangleVAO, m_TriangleShader, m_ModelMatrix, m_Color);
		}
		virtual void OnImGuiRender() override
		{
			ImGui::ColorEdit4("Big Triangle Color", glm::value_ptr(m_Color));
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
		Ref<Shader> m_TriangleShader = nullptr;
		glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
		glm::vec4 m_Color = { 0,1,0,1 };
	public:
		ExampleLayer2()
		{			
			m_ModelMatrix = glm::translate(m_ModelMatrix, { 1,2,0 });
			m_TriangleVAO = std::make_shared<VertexArray>();
			std::string solutionDir = __SOLUTION_DIR;
			m_TriangleShader = ShaderLibrary::GetShader("2DShader");
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
			Renderer::Submit(m_TriangleVAO, m_TriangleShader, m_ModelMatrix, m_Color);
		}
		virtual void OnImGuiRender() override
		{
			ImGui::ColorEdit4("Small Triangle Color", glm::value_ptr(m_Color));
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
		float m_Vertices[7 * 4] =
		{
			//-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			// 1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
			// 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			//-1.0f,  1.0f, 0.0f, 0.0f, 1.0f

			-1.0f, -1.0f, 0.0f, 0.5f, 1.0f, 0.1f,  0.1f,
			 1.0f, -1.0f, 0.0f, 0.4f, 1.0f, 0.1f,  0.1f,
			 1.0f,  1.0f, 0.0f, 0.2f, 1.0f, 0.1f,  0.1f, //example without textures
			-1.0f,  1.0f, 0.0f, 0.1f, 1.0f, 0.1f,   0.1f
		};
		uint32_t m_Indices[6] =
		{
			0, 1, 2, 2, 3, 0
		};
		Ref<VertexArray> m_QuadVAO;
		Ref<Shader> m_QuadShader;
		Ref<Texture> m_QuadTexture;
		glm::mat4 m_QuadModelMatrix = glm::mat4(1.0f);
		glm::vec3 m_QuadTranslation = { 0.0f, 0.0f, 0.0f };
		glm::vec4 m_QuadColor = { 1,0,0,1 };
		float m_QuadMoveSpeed = 5.0f;
		std::string solutionDir = __SOLUTION_DIR;
	public:
		ExampleLayer3()
		{
			m_QuadVAO = std::make_shared<VertexArray>();
			//m_QuadShader = ShaderLibrary::LoadShader("TextureShader", solutionDir + "AnorEngine\\Assets\\Shaders\\2DShader.shader");
			m_QuadShader = ShaderLibrary::GetShader("2DShader");
			if (!m_QuadShader)
			{
				CRITICAL_ASSERT("The Shader you tried to load is invalid. (Either the name is incorrect or there is no shader with that name in the Shader Library.)");
			}
			m_QuadTexture = std::make_shared<Texture>(solutionDir + "AnorEngine\\Assets\\Textures\\transparent.png");
			m_QuadShader->UploadInteger("u_Sampler", 0);
		}
		virtual void OnAttach() override
		{
			BufferLayout QuadBufferLayout = { {ShaderDataType::vec3, "a_Position", 0}, { ShaderDataType::vec4, "a_TexCoord", 1 } };
			m_QuadVAO->AddVertexBuffer(std::make_shared<Buffer>(m_Vertices, 7 * 4 * sizeof(float), QuadBufferLayout));
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

			//Updating quad data here
			m_QuadModelMatrix = glm::translate(m_QuadModelMatrix, m_QuadTranslation);
			//Rendering here 
			m_QuadTexture->Bind();
			Renderer::Submit(m_QuadVAO, m_QuadShader, m_QuadModelMatrix, m_QuadColor);
			m_QuadTexture->Unbind();
			//Reset translation after rendering
			m_QuadTranslation = { 0,0,0 }; 
		}
		virtual void OnImGuiRender() override
		{
			ImGui::ColorEdit4("Quad Color", glm::value_ptr(m_QuadColor));
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



	class SandboxApp : public Application
	{
	private:
		LayerStack m_LayerStack;
		Ref<OrthographicCamera> m_OrthoCamera;
		Ref<OrthographicCameraController> m_OrthoGraphicCameraController;
		Ref<PerspectiveCamera> m_PersCamera;
		Ref<ImGuiBase> m_ImGuiBase = std::make_shared<ImGuiBase>();
		Ref<ExampleLayer>  layer  = std::make_shared<ExampleLayer> ();
		Ref<ExampleLayer2> layer2 = std::make_shared<ExampleLayer2>();
		Ref<ExampleLayer3> layer3 = std::make_shared<ExampleLayer3>();
		std::string solutionDir = __SOLUTION_DIR;
		glm::vec3 cameraPos = { 0.0f, 0.0f, 0.0f };
		float cameraSpeed = 1;
		float lastFrame;
		bool m_Minimized = false;	
#ifdef RENDER_MY_SCENE
		Ref<Scene> scene = std::make_shared<Scene>(m_PersCamera);
#endif
	public:
		SandboxApp()
			:m_OrthoCamera(std::make_shared<OrthographicCamera>(-1280.0f / 720.0f * (5), 1280.0f / 720.0f * (5), -1 * (5), 1 * (5))), m_PersCamera(std::make_shared<PerspectiveCamera>(1280, 720))
		{	
			m_ImGuiBase->Init(); // Need to call the initialization code for imgui here.
			m_OrthoGraphicCameraController = std::make_shared<OrthographicCameraController>(m_OrthoCamera, (1280.0f / 720.0f));
			Input::EventHandler::SetTargetApplication(this); //Important to set this to the active Application else, you won't get your input processed.		
#ifdef RENDER_MY_SCENE
			pushLayer(scene);
#endif		
			//Layer insertion
			pushLayer(layer3);
			pushLayer(layer2);
			pushLayer(layer);
			logInfoDebug();
		}
	protected:
		virtual ~SandboxApp()
		{
			WARN("SandboxApp destructor completed!!!");
		}
	public:
		virtual void Run() override
		{	
			while (!m_OpenGLWindow->IsClosed())
			{
				//Scene setup
				float deltaTime = DeltaTime();
				m_OrthoGraphicCameraController->OnUpdate(deltaTime);			
				Renderer::BeginScene(m_OrthoCamera);
				Renderer::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
				Renderer::Clear();

				//Rendering Layers starts
				if (!m_Minimized) //We don't want to render if the window is minimized.
				{
					for (Ref<Layer> layer : m_LayerStack)
					{	
						layer->OnUpdate(deltaTime);
					}				
				}
				//Rendering Layers ends

				//ImGuiRender starts
					//ImGui Rendering for each layer starts.
				m_ImGuiBase->Begin();
				for (Ref<Layer> layer : m_LayerStack)
				{
					layer->OnImGuiRender();
				}
					//ImGui Rendering for each layer ends.
					//Overlay Rendering ImGui.
				OnImGuiOverlayRender();
				m_ImGuiBase->End();
				//ImGuiRender ends

				m_OpenGLWindow->Update();
				Renderer::EndScene();
			}
		}
		virtual void OnEvent(Ref<Input::Event> e) override
		{
			e->Log();
#ifdef RENDER_MY_SCENE
			if (event.get()->GetEventType() == Input::EventType::KeyPressEvent)
			{
				
				if (std::static_pointer_cast<Input::KeyPressEvent>(event)->GetKeyCode() == ANOR_KEY_LEFT_CONTROL) //static cast doesnt do runtime checks to ensure the casted class is a valid class.
				{
					if (!isMouseCaptured)
					{
						m_OpenGLWindow->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
						isMouseCaptured = true;
						ImGui->wantToCaptureMouse = false;
					}
					else
					{
						m_OpenGLWindow->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
						isMouseCaptured = false;
						isInitialMouseCaptured = false;
						ImGui->wantToCaptureMouse = true;
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
			//Propogating the recieved event to layers.
			//One of the layers can set the 'm_Handled' value of an event to true so that further propogation is prevented.
			for (Ref<Layer> layer : m_LayerStack)
			{
				layer->OnEvent(e);
			}
			if (e->GetEventType() == Input::EventType::WindowResizeEvent)
			{
				OnWindowResizeEvent(std::static_pointer_cast<Input::WindowResizeEvent>(e));
			}
			//Passing every single event that I get from the OpenGLWindow to this controller. Will change.
			m_OrthoGraphicCameraController->OnEvent(e);
		}
		virtual void OnWindowResizeEvent(Ref<Input::WindowResizeEvent> e) override
		{
			if (e->GetHeight() == 0 || e->GetWidth() == 0)
				m_Minimized = true;
			else if (e->GetHeight() > 0 || e->GetWidth() > 0)
				m_Minimized = false;
		}
		virtual void logInfoDebug() override
		{	
			WARN("APP::{0}", "Custom sandbox application has been created!!");
		}
		void OnImGuiOverlayRender()
		{
			static bool show = true;
			ImGui::ShowDemoWindow(&show);
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
			}
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
		return new SandboxApp();
	}
}

