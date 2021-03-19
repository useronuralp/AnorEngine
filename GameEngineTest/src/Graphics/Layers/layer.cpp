#include "pch.h"
#include "Layer.h"

namespace GameEngineTest {
	namespace Graphics
	{
		Layer::Layer()
		{
			m_LayerName = "Layer";
		}
		void Layer::OnUpdate()
		{
			//Do rendering stuff each frame.
		}
		void Layer::OnAttach()
		{
			//Default Stuff upon creation.
		}
		Layer::Layer(const char* name)
			:m_LayerName(name)
		{

		}
	}
}