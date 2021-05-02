#pragma once
#include <GL/glew.h>
#include "buffer.h"
namespace GameEngineTest {
	namespace  Graphics {

		class ENGINE_API VertexArray
		{
		private:
			IndexBuffer m_IndexBuffer;
			GLuint m_ArrayID;
			std::vector<Buffer> m_VertexBuffers;
		public:
			VertexArray();
			~VertexArray();

			void AddVertexBuffer(Buffer& buffer);
			void SetIndexBuffer(IndexBuffer& buffer);
			void bind() const;
			void unbind() const;

			std::vector<Buffer>& GetVertexBuffers() { return m_VertexBuffers; }
			IndexBuffer& GetIndexBuffer() { return m_IndexBuffer; }
		};
	}
}
