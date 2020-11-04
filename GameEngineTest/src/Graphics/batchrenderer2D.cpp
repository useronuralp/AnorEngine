#include "batchrenderer2D.h"


namespace GameEngineTest {
	namespace Graphics {
		BatchRenderer2D::BatchRenderer2D()
		{
			init();
		}

		BatchRenderer2D::~BatchRenderer2D()
		{
			delete m_IBO;
			glDeleteBuffers(1, &m_VBO);
		}
		
		void BatchRenderer2D::begin()
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			m_GiantBuffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		}

		void BatchRenderer2D::submit(const Renderable2D* renderable)
		{
			const Math::vec2 &size = renderable->getSize();
			const Math::vec4 &color = renderable->getColor();
			const Math::vec3 &startPosition = renderable->getPosition();
			const std::vector<Math::vec2>& uv = renderable->getUV();

			int r = color.x * 255.0f;
			int g = color.y * 255.0f;  //faster color operations, study this later.
			int b = color.z * 255.0f;
			int a = color.w * 255.0f;

			unsigned int c = a << 24 | b << 16 | g << 8 | r;


			m_GiantBuffer->vertex = *m_TransformationBack * startPosition;
			m_GiantBuffer->uv = uv[0];
			m_GiantBuffer->color = c;
			m_GiantBuffer++;

			m_GiantBuffer->vertex = *m_TransformationBack * Math::vec3(startPosition.x, startPosition.y + size.y, startPosition.z);
			m_GiantBuffer->uv = uv[1];
			m_GiantBuffer->color = c;
			m_GiantBuffer++;

			m_GiantBuffer->vertex = *m_TransformationBack * Math::vec3(startPosition.x + size.x, startPosition.y + size.y, startPosition.z);
			m_GiantBuffer->uv = uv[2];
			m_GiantBuffer->color = c;
			m_GiantBuffer++;

			m_GiantBuffer->vertex = *m_TransformationBack * Math::vec3(startPosition.x + size.x, startPosition.y, startPosition.z);
			m_GiantBuffer->uv = uv[3];
			m_GiantBuffer->color = c;
			m_GiantBuffer++;

			m_IndexCount += 6;

		}

		void BatchRenderer2D::flush()
		{
			glBindVertexArray(m_VAO);
			m_IBO->bind();

			glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_SHORT, NULL);

			m_IBO->unbind();
			glBindVertexArray(0);

			m_IndexCount = 0;
		}

		void BatchRenderer2D::end()
		{
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void BatchRenderer2D::init()
		{
			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &m_VBO);

			glBindVertexArray(m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
			glEnableVertexAttribArray(SHADER_UV_INDEX);
			glEnableVertexAttribArray(SHADER_COLOR_INDEX);

			glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*) 0);
			glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::uv)));
			glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::color)));
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			GLushort *indices = new GLushort [RENDERER_INDICES_SIZE];

			int offset = 0;
			for (int i = 0; i < RENDERER_INDICES_SIZE; i += 6)
			{
				indices[  i  ] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;
				//there is a certain pattern for this algorithm. Check online if you have difficulty understanding.
				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}

			m_IBO = new IndexBuffer(indices, RENDERER_INDICES_SIZE);

			glBindVertexArray(0);
		}
	}
}
