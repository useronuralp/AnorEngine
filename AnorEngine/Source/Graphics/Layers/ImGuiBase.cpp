#include "pch.h"
#include "ImGuiBase.h"
#include <Core/Application.h>

namespace AnorEngine
{	namespace Graphics
	{
		bool ImGuiBase::Init()
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
			GLFWwindow* window = static_cast<GLFWwindow*>(app.GetOpenGLWindow()->GetNativeWindow());
			
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 460");
			return true;
		}
		void ImGuiBase::Begin()
		{	
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}
		void ImGuiBase::End()
		{
			Application& app = Application::Get();
			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize = ImVec2(app.GetOpenGLWindow()->GetWidth(), app.GetOpenGLWindow()->GetHeight());
		
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
	}
}