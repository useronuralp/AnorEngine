#pragma once
#include "texture.h"
namespace GameEngineTest {
	namespace Graphics {

		class ENGINE_API StaticSprite : public Renderable2D
		{
		private:
			VertexArray* m_VertexArray;
			IndexBuffer* m_IndexBuffer;
			Shader &m_Shader;
			Texture &m_Texture;		
		public:
			StaticSprite(float x, float y, float width, float height, const Math::vec4 &color, Shader &shader, Texture &texture);
			~StaticSprite();

			inline Shader& getShader() const { return m_Shader; }
			inline const VertexArray* getVAO() const { return m_VertexArray; }
			inline const IndexBuffer* getIBO() const { return m_IndexBuffer; }
			inline const Texture getTexture() const { return m_Texture; }
		};
	}
}