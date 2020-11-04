#pragma once
#include "renderable2D.h"
namespace GameEngineTest {
	namespace Graphics {

		class Sprite : public Renderable2D
		{
		private:
		public:
			Sprite(float x, float y, float width, float height, const Math::vec4& color);
		};
	}
}