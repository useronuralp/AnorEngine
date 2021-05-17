#pragma once
#include <GL/glew.h>
#include "Buffer.h"
namespace AnorEngine {
	namespace  Graphics {

		class ANOR_API VertexArray
		{
		private:
			std::vector<Ref<Buffer>> m_VertexBuffers;
			Ref<IndexBuffer> m_IndexBuffer;
			uint32_t m_ArrayID;
		public:
			VertexArray();
			~VertexArray();

			void AddVertexBuffer(const Ref<Buffer>& buffer);
			void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);
			void bind() const;
			void unbind() const;

			const std::vector<Ref<Buffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
			const Ref<IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }
		};
	}
}
