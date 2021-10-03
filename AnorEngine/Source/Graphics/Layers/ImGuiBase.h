#pragma once
#include <glm.hpp>
namespace AnorEngine
{
	struct ANOR_API ViewportVariables
	{
		bool ViewportFocused = false;
		bool ViewportHovered = false;
		bool BlockEvents = false;
		int HoveredPixel = -1;
		glm::vec2 ViewportSize;
		glm::vec2 ViewportBounds[2];
		glm::vec2 MousePositionRelativeToRenderViewport;
	};

	class ANOR_API ImGuiBase
	{
		
	public:
		ImGuiBase() {};
	public:
		static void Begin();
		static void BeginDockSpace();
		static void BeginViewport(ViewportVariables& status);
		static void EndViewport();
		static void EndDockSpace();
		static void End();
		static bool Init();
		static void SetDarkThemeColors();
	};
}