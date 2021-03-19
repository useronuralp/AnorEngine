#include "Engine.h"
namespace Game
{	
	using namespace GameEngineTest;
	using namespace Graphics;
	using namespace Math;

	class Scene : public Layer
	{	
	private:
		Window* m_Window;
		Shader *shader, *shader2, *shader3;
		Renderable3D* light;
		Model *Arianna, *backpack, *basketball;
		glm::mat4 camera;
		Renderer3D renderer3D;
	public:
		Scene(Window *window)
			:m_Window(window)
		{
			
		}
	public:
		void OnAttach() override
		{	
			shader = new Shader("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\src\\shaders\\vertex.shader", "H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\src\\shaders\\fragment.shader");
			shader2 = new Shader("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\src\\shaders\\vertex.shader", "H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\src\\shaders\\lightsourceFrag.shader");
			shader3 = new Shader("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\src\\shaders\\vertex.shader", "H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\src\\shaders\\fragment.shader");


			int layout2[] = { POSITION_VALUE };
			light = new Renderable3D(Shape::lightSourceVertx, 3 * 6 * 6, layout2, 1, 3 * sizeof(float));
			
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
			Arianna = new Model("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\Models\\girl\\Girl_1.obj");
			
			glEnable(GL_DEPTH_TEST);

			basketball->scale(5, 5, 5);
			basketball->translate(0, 0.5f, 0);
			
			camera = glm::lookAt(m_Window->cameraPos, m_Window->cameraPos + m_Window->cameraFront, m_Window->cameraUp);

			Arianna->scale(5, 5, 5);
		}
		void OnUpdate()
		{
			camera = glm::lookAt(m_Window->cameraPos, m_Window->cameraPos + m_Window->cameraFront, m_Window->cameraUp); // camera movement operations on view matrix each frame.
			
			backpack->Draw(*shader3, camera);
			basketball->Draw(*shader3, camera);
			//Arianna.Draw(shader3, camera);
			
			//int i = 0;
			//for (Renderable3D* item : cubes)
			//{
			//	renderer3D.singleDraw(*item, shader, camera, 36);
			//	item->rotate(0.0005f * 10.0f, rotationDirection[i].x, rotationDirection[i].y, rotationDirection[i].z);
			//	rotationDirection[i++];
			//}
			
			renderer3D.singleDraw(*light, *shader2, camera, 36);
		}
	};

	class Sandbox : public Application
	{
	private:
		LayerStack m_LayerStack;
	public:
		Sandbox()
		{	
			pushLayer(new Scene(m_Window));
			pushLayer(new ImGuiLayer());
			logInfoDebug();
		}
		~Sandbox()
		{
			
		}
		void logInfoDebug() override
		{	
			WARN("APP::{0}", "Custom sandbox application has been created!!");
			WARN("Press a enter to continue...");
			getchar();
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
			m_LayerStack.pushLayer(*Layer);
			Layer->OnAttach();
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

