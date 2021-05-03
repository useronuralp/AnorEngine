#include "pch.h"
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
				0,0,0,			   color.x, color.y, color.z, color.w,  getUV()[0].x, getUV()[0].y,
				0,height,0,		   color.x, color.y, color.z, color.w,  getUV()[1].x, getUV()[1].y,
				width, height, 0,  color.x, color.y, color.z, color.w,  getUV()[2].x, getUV()[2].y,
				width,0,0,		   color.x, color.y, color.z, color.w,  getUV()[3].x, getUV()[3].y
			};
			
			//BufferLayout layout = { {ShaderDataType::vec3, "a_Position", 0}, {ShaderDataType::vec2, "a_Uv", 1} , {ShaderDataType::vec4, "a_Color", 3}  };
			//m_VertexArray->AddVertexBuffer(*new Buffer(vertices, 9 * 4, layout)); //This part is not complete, you can code this part as a practice if you want.

			GLushort indices[] = { 0,1,2,2,3,0 };
			m_IndexBuffer = new IndexBuffer((GLuint*)indices, 6);
		}
		StaticSprite::~StaticSprite()
		{
			delete m_VertexArray;
			delete m_IndexBuffer;
		}		
	}
}