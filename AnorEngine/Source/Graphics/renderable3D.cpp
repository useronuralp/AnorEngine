#include "pch.h"
#include "renderable3D.h"
namespace AnorEngine {
	namespace Graphics {

		Renderable3D::Renderable3D(Ref<VertexBuffer> buffer)
			:m_ModelMatrix(glm::mat4(1.0f))
		{	
			init(buffer);
		}	
		void Renderable3D::rotate(const float& degree, const float& x, const float& y, const float& z)
		{
			m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(degree), glm::vec3(x, y, z));
		}
		void Renderable3D::translate(const float& x, const float& y, const float& z)
		{
			m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(x, y, z));
		}
		void Renderable3D::scale(const float& x, const float& y, const float& z)
		{
			m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(x, y, z));
		}
		void Renderable3D::bindVAO()
		{
			m_VAO.Bind();
		}
		void Renderable3D::unbindVAO()
		{
			glBindVertexArray(0);
		}
		void Renderable3D::init(Ref<VertexBuffer> buffer)
		{	
			m_VAO.AddVertexBuffer(buffer);
		}
	}
}