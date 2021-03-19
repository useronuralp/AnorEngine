#pragma once
#include "Layer.h"
#include "../batchrenderer2D.h"

namespace GameEngineTest {
	namespace Graphics {

		class ENGINE_API TileLayer : public Layer
		{
		public:
			TileLayer(Shader* shader);
			virtual ~TileLayer();
		};
	}
}