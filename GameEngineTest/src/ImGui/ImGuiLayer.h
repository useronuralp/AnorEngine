#pragma once
#include "../Graphics/Layers/Layer.h"
#include "../Application.h"
namespace GameEngineTest
{
	namespace Graphics
	{	
		class ENGINE_API ImGuiLayer : public Layer
		{
		private:
		public:
			ImGuiLayer();
			virtual ~ImGuiLayer();
		private:
			void OnAttach() override;
			void OnUpdate() override;
			void OnEvent() override {} 
		public:
			bool OnMouseMove(float xpos, float ypos) override;
			bool OnMouseLeftClickPressedEvent() override;	//the boolean return values in this part represent whether these functions handle the input or not.
			bool OnMouseLeftClickReleasedEvent() override;
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