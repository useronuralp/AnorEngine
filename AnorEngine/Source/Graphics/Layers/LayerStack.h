#pragma once
#include "Layer.h"
namespace AnorEngine {
	namespace Graphics 
	{
		class ANOR_API LayerStack
		{
		public:
			void pushLayer(Ref<Layer> Layer);
			void popLayer();

			std::vector<Ref<Layer>>::reverse_iterator begin() { return m_LayerStack.rbegin(); }
			std::vector<Ref<Layer>>::reverse_iterator end() { return m_LayerStack.rend(); }
		private:
			std::vector<Ref<Layer>> m_LayerStack;
		};
	}
}
