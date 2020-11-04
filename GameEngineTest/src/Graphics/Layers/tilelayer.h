#pragma once
#include "layer.h"
#include "../batchrenderer2D.h"

namespace GameEngineTest {
	namespace Graphics {

		class TileLayer : public Layer
		{
		public:
			TileLayer(Shader* shader);
			virtual ~TileLayer();
		};
	}
}