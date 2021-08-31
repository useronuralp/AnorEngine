#include "pch.h"
#include "ImGuiBase.h"
#include "Core/Application.h"
#include "Core/AnorColors.h"
namespace AnorEngine
{	
	float ImGuiBase::m_Time = 0.0f;
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
		SetDarkThemeColors();

		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		io.Fonts->AddFontFromFileTTF((std::string(__SOLUTION_DIR) + "AnorEngine\\Assets\\Fonts\\OpenSans\\OpenSans-ExtraBold.ttf").c_str(), 21.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF((std::string(__SOLUTION_DIR) + "AnorEngine\\Assets\\Fonts\\OpenSans\\OpenSans-Regular.ttf").c_str(), 21.0f);

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetOpenGLWindow()->GetNativeWindow());
		
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
		return true;
	}
	void ImGuiBase::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		// Headers
		colors[ImGuiCol_Header] = AnorColors::HeaderColor;
		colors[ImGuiCol_HeaderHovered] = AnorColors::HeaderHoveredColor;
		colors[ImGuiCol_HeaderActive] = AnorColors::HeaderActiveColor;

		// Buttons
		colors[ImGuiCol_Button] = AnorColors::ButtonColor;
		colors[ImGuiCol_ButtonHovered] = AnorColors::ButtonHoveredColor;
		colors[ImGuiCol_ButtonActive] = AnorColors::ButtonActiveColor;

		// Frame BG
		colors[ImGuiCol_FrameBg] = AnorColors::FrameBgColor;
		colors[ImGuiCol_FrameBgHovered] = AnorColors::FrameBgHoveredColor;
		colors[ImGuiCol_FrameBgActive] = AnorColors::FrameBgActiveColor;

		// Tabs
		colors[ImGuiCol_Tab] = AnorColors::TabColor;
		colors[ImGuiCol_TabHovered] = AnorColors::TabHoveredColor;
		colors[ImGuiCol_TabActive] = AnorColors::TabActiveColor;
		colors[ImGuiCol_TabUnfocused] = AnorColors::TabUnfocusedColor;
		colors[ImGuiCol_TabUnfocusedActive] = AnorColors::TabUnfocusedActiveColor;

		// Title
		colors[ImGuiCol_TitleBg] = AnorColors::TitleBgColor;
		colors[ImGuiCol_TitleBgActive] = AnorColors::TitleBgActiveColor;
		colors[ImGuiCol_TitleBgCollapsed] = AnorColors::TitleBgCollapsedColor;
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