#pragma once
#include "../Graphics/Layers/Layer.h"
#include "../Application.h"
namespace GameEngineTest
{
	namespace Graphics
	{
		class ENGINE_API ImGuiLayer : public Layer
		{
		public:
			ImGuiLayer();
			virtual ~ImGuiLayer();
		private:
			void OnAttach() override;
			void OnUpdate() override;
			void OnEvent() override;
		private:
			float m_Time = 0.0f;
		private:
			bool MouseButtonPressedEvent();
			bool MouseButtonReleasedEvent();
			bool MouseMovedEvent();
			bool MouseScrollEvent();
			
		};
	}
}