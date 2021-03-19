#include "pch.h"
#include "ImGuiLayer.h"

#include "../ImGuiExampleBuilds/imgui_impl_glfw.h"
#include "../ImGuiExampleBuilds/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>


namespace GameEngineTest
{	namespace Graphics
	{
		ImGuiLayer::ImGuiLayer()
			:Layer("ImGUILayer")
		{
			
		}
		void ImGuiLayer::OnAttach()
		{
			WARN("IMgui on attach!");
			ImGui::CreateContext();
			ImGui::StyleColorsDark();
			
			ImGuiIO& io = ImGui::GetIO();
			
			io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
			io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
			
			ImGui_ImplOpenGL3_Init("#version 460");
		}

		void ImGuiLayer::OnUpdate()
		{

			INFO("ImGUiLayer working!!!");
			
			ImGuiIO& io = ImGui::GetIO();
			Application& app = Application::Get();
			io.DisplaySize = ImVec2(app.getWindow().getWidth() , app.getWindow().getHeight());


			float time = (float)glfwGetTime();
			io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
			m_Time = time;

			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();
			
			
			static bool show = true;
			ImGui::ShowDemoWindow(&show);
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}
}