#include "pch.h"
#include "VertexArray.h"
namespace AnorEngine {
	namespace Graphics {

		VertexArray::VertexArray()
		{
			glGenVertexArrays(1, &m_ArrayID);
		}
		VertexArray::~VertexArray()
		{	
			glDeleteVertexArrays(1, &m_ArrayID);
		}
		void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer)
		{
			glBindVertexArray(m_ArrayID);
			buffer->Bind();
			auto& layout = buffer->GetBufferLayout();
			for (const auto& element : layout) //This is a vector.
			{
				glEnableVertexAttribArray(element.m_ShaderLayoutLocation);
				glVertexAttribPointer(element.m_ShaderLayoutLocation, element.m_ComponentCount, ShaderDataTypeToOpenGLBaseType(element.m_Type), element.m_Normalized ? GL_TRUE : GL_FALSE, buffer->GetBufferLayout().GetStride(), (void*)element.m_Offset);
			}
			m_VertexBuffers.push_back(buffer);
			buffer->Unbind();
			glBindVertexArray(0);
		}
		void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
		{
			glBindVertexArray(m_ArrayID);
			indexBuffer->Bind();

			m_IndexBuffer = indexBuffer;
			glBindVertexArray(0);
		}
		void VertexArray::Bind() const
		{
			glBindVertexArray(m_ArrayID);
		}
		void VertexArray::Unbind() const
		{
			glBindVertexArray(0);
		}
	}
}