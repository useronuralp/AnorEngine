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

			std::vector<Ref<Layer>>::iterator rbegin() { return m_LayerStack.begin(); }
			std::vector<Ref<Layer>>::iterator rend() { return m_LayerStack.end(); }
		private:
			std::vector<Ref<Layer>> m_LayerStack;
		};
	}
}
