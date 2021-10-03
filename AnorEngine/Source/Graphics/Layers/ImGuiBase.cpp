#include "pch.h"
#include "ImGuiBase.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Core/Application.h"
#include "Graphics/OpenGLWindow.h"
#include "Core/AnorColors.h"
#include "Graphics/Framebuffer.h"
#include "Renderer/Renderer2D.h"
#include <type_traits>
#include <GLFW/glfw3.h>

namespace AnorEngine
{	
	static bool dockspaceOpen = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

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
	void ImGuiBase::BeginDockSpace()
	{
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		auto& style = ImGui::GetStyle();
		//float minWinSizeX = style.WindowMinSize.x;
		//style.WindowMinSize.x = 420.0f;
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		//style.WindowMinSize.x = minWinSizeX;

	}
	void ImGuiBase::BeginViewport(ViewportVariables& status)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin("Viewport");
		status.ViewportFocused = ImGui::IsWindowFocused();
		status.ViewportHovered = ImGui::IsWindowHovered();
		status.BlockEvents = !status.ViewportFocused || !status.ViewportHovered;
		//Saving the viewport size in m_ViewportSize.
		status.ViewportSize.x = ImGui::GetContentRegionAvail().x;
		status.ViewportSize.y = ImGui::GetContentRegionAvail().y;

		//Mouse picking calculations.
		auto viewPortOffset = ImGui::GetCursorPos();
		auto windowSize = ImGui::GetWindowSize();
		auto minBound = ImGui::GetWindowPos();
		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		minBound.x += viewPortOffset.x;
		minBound.y += viewPortOffset.y;
		status.ViewportBounds[0] = { minBound.x, minBound.y };
		status.ViewportBounds[1] = { maxBound.x, maxBound.y };
		auto [mx, my] = ImGui::GetMousePos();
		mx -= status.ViewportBounds[0].x;
		my -= status.ViewportBounds[0].y;
		my = status.ViewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;
		status.MousePositionRelativeToRenderViewport.x = mouseX;
		status.MousePositionRelativeToRenderViewport.y = mouseY;
		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)status.ViewportSize.x && mouseY < (int)status.ViewportSize.y)
		{
			Graphics::Renderer2D::GetMainFramebuffer()->Bind();
			int pixelData = Graphics::Renderer2D::GetMainFramebuffer()->ReadPixel(1, mouseX, mouseY);
			WARN(pixelData);
			status.HoveredPixel = pixelData;
			Graphics::Renderer2D::GetMainFramebuffer()->Unbind();
		}
	}
	void ImGuiBase::EndViewport()
	{
		ImGui::End();
		ImGui::PopStyleVar();
	}
	void ImGuiBase::EndDockSpace()
	{
		ImGui::End();
	}
}