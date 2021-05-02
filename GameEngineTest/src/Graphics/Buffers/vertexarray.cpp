#include "pch.h"
#include "VertexArray.h"
namespace GameEngineTest {
	namespace Graphics {


		VertexArray::VertexArray()
		{
			glGenVertexArrays(1, &m_ArrayID);
		}
		VertexArray::~VertexArray()
		{
			glDeleteVertexArrays(1, &m_ArrayID);
		}
		void VertexArray::AddVertexBuffer(Buffer& buffer)
		{
			glBindVertexArray(m_ArrayID);//THIS PART IS SO IMPORTANT. OpenGL is a state machine and in this code snippet, before linking buffer data with a VAO, you FIRST NEED TO BIND THE VAO. Meaning you need to select the state.
			buffer.bind(); //Remember this binding operation is done by using the m_ArrayID which is unique to every array. After binding the array, you need to bind a buffer. Then link the two active objects together using the
			const auto& layout = buffer.getBufferLayout().GetBufferElements();
			for (auto& element : layout)
			{
				glEnableVertexAttribArray(element.ShaderLayoutLocation);
				glVertexAttribPointer(element.ShaderLayoutLocation, element.componentCount, ShaderDataTypeToOpenGLBaseType(element.type), element.normalized ? GL_TRUE : GL_FALSE, buffer.getBufferLayout().getStride(), (void*)element.offset);
			}
			m_VertexBuffers.push_back(buffer);
			buffer.unbind();
			glBindVertexArray(0);
		}
		void VertexArray::SetIndexBuffer(IndexBuffer& indexBuffer)
		{
			glBindVertexArray(m_ArrayID);
			indexBuffer.bind();

			m_IndexBuffer = indexBuffer;
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