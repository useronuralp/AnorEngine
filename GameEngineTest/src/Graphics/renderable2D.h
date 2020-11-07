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
			//VertexArray* m_VertexArray;
			//IndexBuffer* m_IndexBuffer;
			//Shader &m_Shader;
		protected:
			Renderable2D(){ setUVDefaults(); }
		public:
			Renderable2D(Math::vec3 position, Math::vec2 size, Math::vec4 color)
				:m_Position(position), m_Size(size), m_Color(color)
			{

				setUVDefaults();


				//m_VertexArray = new VertexArray();
				//GLfloat vertices[] =
				//{
				//	0,0,0,
				//	0,size.y,0,
				//	size.x, size.y, 0,
				//	size.x,0,0
				//};
				//
				//GLfloat colors[] =
				//{
				//	color.x, color.y, color.z, color.w,
				//	color.x, color.y, color.z, color.w,
				//	color.x, color.y, color.z, color.w,
				//	color.x, color.y, color.z, color.w
				//};
				//
				//m_VertexArray->addBuffer(new Buffer(vertices, 4 * 3, 3), 0);
				//m_VertexArray->addBuffer(new Buffer(colors, 4 * 4, 4), 1);
				//
				//GLushort indices[] = { 0,1,2,2,3,0 };
				//m_IndexBuffer = new IndexBuffer(indices, 6);
			}
			virtual ~Renderable2D()
			{
				//delete m_VertexArray;
				//delete m_IndexBuffer;
			}
			//inline Shader& getShader() const { return m_Shader; }
			//inline const VertexArray* getVAO() const { return m_VertexArray; }
			//inline const IndexBuffer* getIBO() const { return m_IndexBuffer; }

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