#pragma once
#include "renderable2D.h"
namespace GameEngineTest {
	namespace Graphics {

		class ENGINE_API Sprite : public Renderable2D
		{
		private:
		public:
			Sprite(float x, float y, float width, float height, const Math::vec4& color);
			Sprite(float x, float y, float width, float height, Texture* texture);
		};
	}
}