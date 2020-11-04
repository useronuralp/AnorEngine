#include "renderer2D.h"
#include "static_sprite.h"

namespace GameEngineTest {
	namespace Graphics {
		StaticSprite::StaticSprite(float x, float y, float width, float height, const Math::vec4& color, Shader &shader, Texture &texture)
			:Renderable2D(Math::vec3(x, y, 0), Math::vec2(width, height), color), m_Shader(shader), m_Texture(texture)
		{	
			m_VertexArray = new VertexArray();
			GLfloat vertices[] =
			{
				0,0,0,
				0,height,0,
				width, height, 0,
				width,0,0
			};
			
			GLfloat colors[] =
			{
				color.x, color.y, color.z, color.w,
				color.x, color.y, color.z, color.w,
				color.x, color.y, color.z, color.w,
				color.x, color.y, color.z, color.w
			};			

			GLfloat uv[] =
			{
				getUV()[0].x, getUV()[0].y,
				getUV()[1].x, getUV()[1].y,
				getUV()[2].x, getUV()[2].y,
				getUV()[3].x, getUV()[3].y,
			};

			m_VertexArray->bindBuffer(new Buffer(vertices, 3 * 4, 3), 0);
			m_VertexArray->bindBuffer(new Buffer(uv, 2 * 4, 2), 1);
			m_VertexArray->bindBuffer(new Buffer(colors, 4 * 4, 4), 2);

			GLushort indices[] = { 0,1,2,2,3,0 };
			m_IndexBuffer = new IndexBuffer(indices, 6);
		}
		StaticSprite::~StaticSprite()
		{
			delete m_VertexArray;
			delete m_IndexBuffer;
		}		
	}
}