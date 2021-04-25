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
			WARN("ImGUI default constructor that calls 'Layer(const char* name)'");
		}
		void ImGuiLayer::OnAttach()
		{
			WARN("ImGUI on attach!");
			ImGui::CreateContext();
			ImGui::StyleColorsDark();
			
			ImGuiIO& io = ImGui::GetIO();
			
			io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
			io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
			
			ImGui_ImplOpenGL3_Init("#version 460");
		}

		void ImGuiLayer::OnUpdate()
		{

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

		bool ImGuiLayer::OnMouseLeftClickPressedEvent()
		{
			ImGuiIO& io = ImGui::GetIO();
			if (io.WantCaptureMouse)
			{
				io.MouseDown[0] = true;
				return true; // means this fnc handles the input
			}
			return false;
		}
		bool ImGuiLayer::OnMouseLeftClickReleasedEvent()
		{	
			ImGuiIO& io = ImGui::GetIO();
			if (io.WantCaptureMouse)
			{
				io.MouseDown[0] = false;
				return true;
			}
			return false;
		}
		bool ImGuiLayer::OnMouseMove(float xpos, float ypos)
		{	
			ImGuiIO& io = ImGui::GetIO();

			if (ImGui::IsAnyItemHovered())
				io.WantCaptureMouse = true;
			else
				io.WantCaptureMouse = false;

			if (io.WantCaptureMouse)
			{
				WARN("IMGUI is handling the input right now: x = {0}, y = {1}", io.MousePos.x, io.MousePos.y);
				io.MousePos = ImVec2(xpos, ypos);
				return true;
			}
			else
			{
				io.MousePos = ImVec2(xpos, ypos);
				return false;
			}		
		}
		ImGuiLayer::~ImGuiLayer()
		{
			WARN("ImGuiLayer Destructor completed!!!");
		}
		bool ImGuiLayer::MouseButtonPressedEvent()
		{	
			return false;
		}
		bool ImGuiLayer::MouseButtonReleasedEvent()
		{
			return false;
		}
		bool ImGuiLayer::MouseMovedEvent()
		{
			return false;
		}
		bool ImGuiLayer::MouseScrollEvent()
		{
			return false;
		}
	}
}