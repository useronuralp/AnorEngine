#include "pch.h"
#include "LayerStack.h"

namespace GameEngineTest {
	namespace Graphics
	{
		void LayerStack::pushLayer(Layer &Layer)
		{
			m_LayerStack.push_back(&Layer);
		}
		void LayerStack::popLayer()
		{
			m_LayerStack.pop_back();
		}
	}
}