#pragma once
#include <glm.hpp>
#include <type_traits>
#include <ImGuiExampleBuilds/imgui_impl_glfw.h>
#include <ImGuiExampleBuilds/imgui_impl_opengl3.h>
namespace AnorEngine
{
	namespace Graphics
	{	
		class ANOR_API ImGuiBase
		{
		public:
			ImGuiBase() {};
		public:
			void Begin();
			void End();
			bool Init();
		private:
			float m_Time = 0.0f;
		};
	}
}