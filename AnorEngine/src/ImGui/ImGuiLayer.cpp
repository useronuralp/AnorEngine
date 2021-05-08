#include "pch.h"
#include "ImGuiLayer.h"
#include "../Application.h" //added fresh
#include "../ImGuiExampleBuilds/imgui_impl_glfw.h"
#include "../ImGuiExampleBuilds/imgui_impl_opengl3.h"
namespace AnorEngine
{	namespace Graphics
	{
		ImGuiLayer::ImGuiLayer()
			:Layer("ImGUILayer")
		{
			WARN("ImGUI default constructor that calls 'Layer(const char* name)'");
		}
		void ImGuiLayer::OnAttach()
		{	
			
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			//Setup Dear ImGui style
			ImGui::StyleColorsDark();

			ImGuiStyle& style = ImGui::GetStyle();

			if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			Application& app = Application::Get();
			GLFWwindow* window = static_cast<GLFWwindow*>(app.GetOpenGLWindow().GetNativeWindow());
			
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 460");
		}
		void ImGuiLayer::OnUpdate()
		{
			Begin();

			static bool show = true;
			ImGui::ShowDemoWindow(&show);
			
			End();
		}
		void ImGuiLayer::Begin()
		{	
			ImGuiIO& io = ImGui::GetIO();
			if (!wantToCaptureMouse) //sets whether the mouse input will be captured by imgui. If the mouse is inivisible, that means the user interacts with the game scene so disable the imgui input.
			{
				io.ConfigFlags |= ImGuiConfigFlags_NoMouse; 
			}
			else
			{
				io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
			}
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}
		void ImGuiLayer::End()
		{
			Application& app = Application::Get();
			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize = ImVec2(app.GetOpenGLWindow().GetWidth(), app.GetOpenGLWindow().GetHeight());
		
			//Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
		}
		ImGuiLayer::~ImGuiLayer()
		{
			WARN("ImGuiLayer Destructor completed!!!");
		}
	}
}