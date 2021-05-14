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

			std::vector<Ref<Layer>>::iterator begin() { return m_LayerStack.begin(); }
			std::vector<Ref<Layer>>::iterator end() { return m_LayerStack.end(); }
		private:
			std::vector<Ref<Layer>> m_LayerStack;
		};
	}
}
