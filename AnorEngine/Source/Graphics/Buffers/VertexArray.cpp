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
				switch (element.m_Type)
				{
					//Multiple cases use the same body.
					case ShaderDataType::Int:
					case ShaderDataType::Int2:
					case ShaderDataType::Int3:
					case ShaderDataType::Int4:
					case ShaderDataType::Bool:
					{
						glEnableVertexAttribArray(element.m_ShaderLayoutLocation);
						glVertexAttribIPointer(element.m_ShaderLayoutLocation, element.m_ComponentCount, ShaderDataTypeToOpenGLBaseType(element.m_Type),
							buffer->GetBufferLayout().GetStride(), (void*)element.m_Offset);
						break;
					}
					case ShaderDataType::vec:
					case ShaderDataType::vec2:
					case ShaderDataType::vec3:
					case ShaderDataType::vec4:
					{
						glEnableVertexAttribArray(element.m_ShaderLayoutLocation);
						glVertexAttribPointer(element.m_ShaderLayoutLocation, element.m_ComponentCount, 
							ShaderDataTypeToOpenGLBaseType(element.m_Type), element.m_Normalized ? GL_TRUE : GL_FALSE, buffer->GetBufferLayout().GetStride(), (void*)element.m_Offset);
						break;
					}
				}
				
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