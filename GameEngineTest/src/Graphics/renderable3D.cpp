#include "pch.h"
#include "renderable3D.h"
namespace GameEngineTest {
	namespace Graphics {

		Renderable3D::Renderable3D(std::shared_ptr<Buffer> buffer)
			:modelMatrix(glm::mat4(1.0f)), viewMatrix(glm::mat4(1.0f)) , projectionMatrix(glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.01f, 1000.0f)) //TODO: projection matrix is hard coded, look into it later.
		{	
			init(buffer);
		}	
		void Renderable3D::rotate(const float& degree, const float& x, const float& y, const float& z)
		{
			modelMatrix = glm::rotate(modelMatrix, glm::radians(degree), glm::vec3(x, y, z));
		}
		void Renderable3D::translate(const float& x, const float& y, const float& z)
		{
			modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
		}
		void Renderable3D::scale(const float& x, const float& y, const float& z)
		{
			modelMatrix = glm::scale(modelMatrix, glm::vec3(x, y, z));
		}
		void Renderable3D::bindVAO()
		{
			m_VAO.bind();
		}
		void Renderable3D::unbindVAO()
		{
			glBindVertexArray(0);
		}
		void Renderable3D::init(std::shared_ptr<Buffer> buffer)
		{	
			m_VAO.AddVertexBuffer(buffer);
		}
	}
}