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
			//if(!m_IndexBuffer)
			//	delete m_IndexBuffer;
			//for (auto buffer : m_VertexBuffers)
			//{
			//	delete buffer; //This is not managed by the shader_ptr class.
			//}
			glDeleteVertexArrays(1, &m_ArrayID);
		}
		void VertexArray::AddVertexBuffer(const Ref<Buffer>& buffer)
		{
			glBindVertexArray(m_ArrayID);
			buffer->bind();
			auto& layout = buffer->GetBufferLayout();
			for (const auto& element : layout) //This is a vector.
			{
				glEnableVertexAttribArray(element.m_ShaderLayoutLocation);
				glVertexAttribPointer(element.m_ShaderLayoutLocation, element.m_ComponentCount, ShaderDataTypeToOpenGLBaseType(element.m_Type), element.m_Normalized ? GL_TRUE : GL_FALSE, buffer->GetBufferLayout().GetStride(), (void*)element.m_Offset);
			}
			m_VertexBuffers.push_back(buffer);
			buffer->unbind();
			glBindVertexArray(0);
		}
		void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
		{
			glBindVertexArray(m_ArrayID);
			indexBuffer->bind();

			m_IndexBuffer = indexBuffer;
			glBindVertexArray(0);
		}
		void VertexArray::bind() const
		{
			glBindVertexArray(m_ArrayID);
		}
		void VertexArray::unbind() const
		{
			glBindVertexArray(0);
		}
	}
}