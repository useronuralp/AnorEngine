#include "Engine.h"
namespace Game
{	
	using namespace GameEngineTest;
	using namespace Graphics;
	using namespace Math;

	class Scene : public Layer
	{	
	private:
		OpenGLWindow* m_Window = nullptr;
		Shader *shader = nullptr, *shader2 = nullptr, *shader3 = nullptr;
		Renderable3D *light = nullptr;
		Model *Arianna = nullptr, *backpack = nullptr, *basketball = nullptr;
		glm::mat4 camera;
		Renderer3D renderer3D;
	public:
		Scene(OpenGLWindow *window)
			:Layer("Scene"), m_Window(window)
		{
			WARN("Scene default constrcutor that calls 'Layer(const char* name)'");
		}
	public:
		void OnAttach() override
		{	
			shader = new Shader("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\src\\shaders\\vertex.shader", "H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\src\\shaders\\fragment.shader");
			shader2 = new Shader("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\src\\shaders\\vertex.shader", "H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\src\\shaders\\lightsourceFrag.shader");
			shader3 = new Shader("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\src\\shaders\\vertex.shader", "H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\src\\shaders\\fragment.shader");
		



			BufferLayout layout = { {ShaderDataType::vec3, "a_Position", 0} };
			
			uint32_t totalComponentCount = sizeof(Shape::lightSourceVertx) / sizeof(Shape::lightSourceVertx[0]); // this is how you calculate the element count in a const float* array.
			uint32_t size = totalComponentCount * sizeof(GL_FLOAT);

			light = new Renderable3D(*new Buffer(Shape::lightSourceVertx, size ,layout));



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
			
			backpack = new Model("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\Models\\backpack\\backpack.obj");
			basketball = new Model("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\Models\\ball\\uploads_files_2222080_ball_obj.obj");
			//Arianna = new Model("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\Models\\girl\\Girl_1.obj");

			glEnable(GL_DEPTH_TEST);
			
			basketball->scale(5, 5, 5);
			basketball->translate(0, 0.5f, 0);
			
			camera = glm::lookAt(m_Window->cameraPos, m_Window->cameraPos + m_Window->cameraFront, m_Window->cameraUp);
			
			//Arianna->scale(5, 5, 5);
			//Arianna->translate(0, 0, -0.7f);
		}
		void OnUpdate() override
		{
			camera = glm::lookAt(m_Window->cameraPos, m_Window->cameraPos + m_Window->cameraFront, m_Window->cameraUp); // camera movement operations on view matrix each frame.
			
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
			WARN("Scene Destructor completed!!");
		}
	};

	class Sandbox : public Application
	{
	private:
		LayerStack m_LayerStack;
	public:
		Sandbox()
		{	
			pushLayer(new Scene(m_Window)); //as our first layer we need to create the Scene that our "game" will play out on.
			m_Window->setImGuiPointer(new ImGuiLayer()); //after creating the scene layer. We need to attach a new ImGuiLayer as a debug layer/ui to its member variable. (This part should be changed later)
			pushLayer(m_Window->getImGuiWindowPointer()); // finally, we push the debug/imgui layer to our layer stack to render it. (This layer should be rendered first, meaning it should be pushed into the stack as the last element.)
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
			while (!m_Window->isClosed())
			{									
				m_Window->clear();
				for (Layer* layer : m_LayerStack)
				{	
					layer->OnUpdate();
				}
				m_Window->update();
			}
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
	};
	GameEngineTest::Application* CreateApplication()
	{
		return new Sandbox();
	}
}

