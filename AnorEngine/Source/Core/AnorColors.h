#pragma once
#include <imgui.h>

namespace AnorEngine
{
	namespace AnorColors
	{
		//Buttons
		static ImVec4 DefaultButtonColor = { 0.0f, 0.0f, 0.0f, 0.0f };
		static ImVec4 ButtonColor = { 0.2f, 0.205f, 0.21f, 1.0f };
		static ImVec4 ButtonHoveredColor = { 0.3f, 0.305f, 0.31f, 1.0f };
		static ImVec4 ButtonActiveColor =  { 0.15f, 0.1505f, 0.151f, 1.0f };

		//Header
		static ImVec4 HeaderColor = { 0.01f, 0.2f, 0.4f, 1.0f };
		static ImVec4 HeaderHoveredColor = { 0.3f, 0.4f, 0.6f, 1.0f };
		static ImVec4 HeaderActiveColor = { 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		static ImVec4 FrameBgColor = { 0.2f, 0.205f, 0.21f, 1.0f };
		static ImVec4 FrameBgHoveredColor = { 0.3f, 0.305f, 0.31f, 1.0f };
		static ImVec4 FrameBgActiveColor = { 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		static ImVec4 TabColor ={ 0.15f, 0.1505f, 0.151f, 1.0f };
		static ImVec4 TabHoveredColor = { 0.38f, 0.3805f, 0.381f, 1.0f };
		static ImVec4 TabActiveColor = { 0.28f, 0.2805f, 0.281f, 1.0f };
		static ImVec4 TabUnfocusedColor = { 0.15f, 0.1505f, 0.151f, 1.0f };
		static ImVec4 TabUnfocusedActiveColor{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		static ImVec4 TitleBgColor = { 0.15f, 0.1505f, 0.151f, 1.0f };
		static ImVec4 TitleBgActiveColor = { 0.15f, 0.1505f, 0.151f, 1.0f };
		static ImVec4 TitleBgCollapsedColor = { 0.15f, 0.1505f, 0.151f, 1.0f };
	}
}

