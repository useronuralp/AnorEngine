#include "pch.h"
#include "vertexarray.h"
namespace GameEngineTest {
	namespace Graphics {

		VertexArray::VertexArray()
		{
			glGenVertexArrays(1, &m_ArrayID);
		}
		VertexArray::~VertexArray()
		{
		}

		void VertexArray::bindBuffer(Buffer* buffer, GLuint index)
		{
			bind();//THIS PART IS SO IMPORTANT. OpenGL is a state machine and in this code snippet, before linking buffer data with a VAO, you FIRST NEED TO BIND THE VAO. Meaning you need to select the state.
			buffer->bind(); //Remember this binding operation is done by using the m_ArrayID which is unique to every array. After binding the array, you need to bind a buffer. Then link the two active objects together using the
			glEnableVertexAttribArray(index);//following two lines of gl calls.
			glVertexAttribPointer(index, buffer->getComponentCount(), GL_FLOAT, GL_FALSE, 0, 0);
			buffer->unbind();
			unbind();
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