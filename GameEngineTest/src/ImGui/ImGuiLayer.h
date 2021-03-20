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
		private:
			float m_Time = 0.0f;
		};
	}
}