#include "pch.h"
#include "sprite.h"
#include "../Math/maths.h"

namespace AnorEngine {
	namespace Graphics {
		Sprite::Sprite(float x, float y, float width, float height, const Math::vec4& color)
			:Renderable2D(Math::vec3(x, y, 0), Math::vec2(width, height), color)
		{

		}
		Sprite::Sprite(float x, float y, float width, float height, Texture* texture)
			: Renderable2D(Math::vec3(x, y, 0), Math::vec2(width, height), Math::vec4(1, 0, 1, 1))
		{
			 m_Texture = texture;
		}
	}
}