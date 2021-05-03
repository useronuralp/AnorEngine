#pragma once
#include "../Graphics/Layers/Layer.h"
namespace GameEngineTest
{
	namespace Graphics
	{	
		class ENGINE_API ImGuiLayer : public Layer
		{
		private:
		public:
			bool wantToCaptureMouseInput = true;
			ImGuiLayer();
			virtual ~ImGuiLayer();
		private:
			void OnAttach() override;
			void OnUpdate() override;
			void OnEvent() override {} 
		private:
			float m_Time = 0.0f;
		public:
			void Begin();
			void End();
		};
	}
}