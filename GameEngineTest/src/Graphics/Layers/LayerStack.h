#pragma once
#include "Layer.h"
namespace GameEngineTest {
	namespace Graphics 
	{
		class ENGINE_API LayerStack
		{
		public:
			void pushLayer(Layer &Layer);
			void popLayer();

			std::vector<Layer*>::iterator begin() { return m_LayerStack.begin(); }
			std::vector<Layer*>::iterator end() { return m_LayerStack.end(); }
		private:
			std::vector<Layer*> m_LayerStack;
		};
	}
}
