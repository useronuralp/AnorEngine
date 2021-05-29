#include "pch.h"
#include "Buffer.h"
namespace AnorEngine{
	namespace Graphics {
		//----------------------------------------------
		//-----------------------------------------------
		//-------------VertexBuffer impl ----------------
		//----------------------------------------------
		//-----------------------------------------------
		Buffer::Buffer(float* vertices, uint32_t sizeByte, BufferLayout& layout)
			:m_Layout(layout), m_SizeByte(sizeByte)
		{
			glGenBuffers(1, &m_BufferID);
			glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
			glBufferData(GL_ARRAY_BUFFER,  m_SizeByte, vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		Buffer::~Buffer()
		{	
			glDeleteBuffers(1, &m_BufferID);
		}
		void Buffer::bind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
		}
		void Buffer::unbind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		//----------------------------------------------
		//-----------------------------------------------
		//-------------IndexBuffer impl ----------------
		//----------------------------------------------
		//-----------------------------------------------
		IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
			:m_Count(count)
		{
			glGenBuffers(1, &m_BufferID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		IndexBuffer::~IndexBuffer()
		{
			glDeleteBuffers(1, &m_BufferID);
		}
		void IndexBuffer::bind() const
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
		}
		void IndexBuffer::unbind() const
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		
	}
}