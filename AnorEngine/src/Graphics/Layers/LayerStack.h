#pragma once
#include "Layer.h"
namespace AnorEngine {
	namespace Graphics 
	{
		class ANOR_API LayerStack
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
