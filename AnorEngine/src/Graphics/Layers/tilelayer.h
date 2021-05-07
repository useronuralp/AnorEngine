#pragma once
#include "Layer.h"
#include "../batchrenderer2D.h"

namespace AnorEngine {
	namespace Graphics {

		class ANOR_API TileLayer : public Layer
		{
		public:
			TileLayer(Shader* shader);
			virtual ~TileLayer();
		};
	}
}