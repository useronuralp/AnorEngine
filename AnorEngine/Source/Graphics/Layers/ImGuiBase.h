#pragma once
#include <glm.hpp>
#include <type_traits>
#include <ImGuiExampleBuilds/imgui_impl_glfw.h>
#include <ImGuiExampleBuilds/imgui_impl_opengl3.h>
namespace AnorEngine
{
	class ANOR_API ImGuiBase
	{
	public:
		ImGuiBase() {};
	public:
		static void Begin();
		static void End();
		static bool Init();
		static void SetDarkThemeColors();
	private:
		static float m_Time;
	};
}