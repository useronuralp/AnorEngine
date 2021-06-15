#pragma once
#include "Buffer.h"
namespace AnorEngine {
	namespace  Graphics {

		class ANOR_API VertexArray
		{
		private:
			std::vector<Ref<VertexBuffer>> m_VertexBuffers;
			Ref<IndexBuffer> m_IndexBuffer;
			uint32_t m_ArrayID;
		public:
			VertexArray();
			~VertexArray();

			void AddVertexBuffer(const Ref<VertexBuffer>& buffer);
			void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);
			void Bind() const;
			void Unbind() const;

			const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
			const Ref<IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }
		};
	}
}
