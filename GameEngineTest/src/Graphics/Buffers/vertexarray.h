#pragma once
#include <GL/glew.h>
#include "Buffer.h"
namespace GameEngineTest {
	namespace  Graphics {

		class ENGINE_API VertexArray
		{
		private:
			std::vector<std::shared_ptr<Buffer>> m_VertexBuffers;
			std::shared_ptr<IndexBuffer> m_IndexBuffer;
			uint32_t m_ArrayID;
		public:
			VertexArray();
			~VertexArray();

			void AddVertexBuffer(const std::shared_ptr<Buffer>& buffer);
			void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);
			void bind() const;
			void unbind() const;

			const std::vector<std::shared_ptr<Buffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
			const IndexBuffer& GetIndexBuffer() const { return *m_IndexBuffer; }
		};
	}
}
