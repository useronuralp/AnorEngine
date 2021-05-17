#pragma once
#include "../Graphics/Layers/Layer.h"
#include <glm.hpp>
#include <type_traits>
#include "../ImGuiExampleBuilds/imgui_impl_glfw.h"
#include "../ImGuiExampleBuilds/imgui_impl_opengl3.h"
namespace AnorEngine
{
	namespace Graphics
	{	
		class ANOR_API ImGuiLayer : public Layer
		{
		private:
		public:
			glm::vec4* color;
			bool wantToCaptureMouseInput = true;
			ImGuiLayer();
			virtual ~ImGuiLayer();
		private:
			virtual void OnAttach() override;
			virtual void OnUpdate() override {} ;
			virtual void OnEvent() override {} ;
		public:
			void OnImGuiRender() override;
		private:
			float m_Time = 0.0f;
		public:
			void Begin();
			void End();
		};
	}
}