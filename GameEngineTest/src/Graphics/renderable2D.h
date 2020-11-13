#pragma once
#include "Buffers/indexbuffer.h"
#include "Buffers/vertexarray.h"
#include "../Math/maths.h"
#include "../Graphics/shader.h"
#include "texture.h"

namespace GameEngineTest {
	namespace Graphics {

		struct VertexData
		{
			Math::vec3 vertex;
			//Math::vec4 color;
			Math::vec2 uv; //texture coordinates.
			float tid; //texture id.
			unsigned int color;
		};
		class Renderable2D
		{
		protected:
			Math::vec3 m_Position;
			Math::vec2 m_Size;
			Math::vec4 m_Color;
			std::vector<Math::vec2> m_UV;
			Texture* m_Texture;
		protected:
			Renderable2D(){ setUVDefaults(); }
		public:
			Renderable2D(Math::vec3 position, Math::vec2 size, Math::vec4 color)
				:m_Position(position), m_Size(size), m_Color(color)
			{

				setUVDefaults();

			}
			virtual ~Renderable2D()
			{
			}
			inline const Math::vec3& getPosition() const { return m_Position; }
			inline const Math::vec2& getSize() const { return m_Size; }
			inline const Math::vec4& getColor() const { return m_Color; }
			inline const void setPosition(const Math::vec3& position) { m_Position = position; }
			inline const std::vector<Math::vec2>& getUV() const { return m_UV; }
			inline const GLuint getTextureID() const { return m_Texture == nullptr ? 0 : m_Texture->getTID(); }
		private:
			void setUVDefaults()
			{
				m_UV.push_back(Math::vec2(0, 0));
				m_UV.push_back(Math::vec2(0, 1));
				m_UV.push_back(Math::vec2(1, 1));
				m_UV.push_back(Math::vec2(1, 0));
			}
		};
	}
}