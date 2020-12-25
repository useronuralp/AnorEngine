#include "renderable3D.h"

namespace GameEngineTest {
	namespace Graphics {

		Renderable3D::Renderable3D(const float* vertex, const int& vertexComponentCount, int* layout, const int& layoutComponentCount, const int& stride)
			:m_VBO(0),m_VAO(0), modelMatrix(glm::mat4(1.0f)), viewMatrix(glm::mat4(1.0f)) , projectionMatrix(glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f)) //TODO: projection matrix is hard coded, look into it later.
		{	
			init(vertex, vertexComponentCount, layout, layoutComponentCount, stride);
		}
		Renderable3D::Renderable3D(Model &model, const int& vertexComponentCount, int* layout, const int& layoutComponentCount, const int& stride)
			:m_VBO(0), m_VAO(0), modelMatrix(glm::mat4(1.0f)), viewMatrix(glm::mat4(1.0f)), projectionMatrix(glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f))
		{
			
			std::vector<float> vertex;
			for (int i = 0; i < model.getMeshes().size(); i++)
			{
				for (int j = 0; j < model.getMeshes()[i].vertices.size(); j++)
				{
					
					vertex.push_back(model.getMeshes()[i].vertices[j].Position.x);
					vertex.push_back(model.getMeshes()[i].vertices[j].Position.y);
					vertex.push_back(model.getMeshes()[i].vertices[j].Position.z);

					vertex.push_back(model.getMeshes()[i].vertices[j].TexCoords.x);
					vertex.push_back(model.getMeshes()[i].vertices[j].TexCoords.y);

					vertex.push_back(model.getMeshes()[i].vertices[j].Normal.x);
					vertex.push_back(model.getMeshes()[i].vertices[j].Normal.y);
					vertex.push_back(model.getMeshes()[i].vertices[j].Normal.z);
				}

			}		
			init(&vertex[0], vertexComponentCount, layout, layoutComponentCount, stride);

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
			glBindVertexArray(m_VAO);
		}
		void Renderable3D::unbindVAO()
		{
			glBindVertexArray(0);
		}
		void Renderable3D::bindVBO()
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		}
		void Renderable3D::unbindVBO()
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		void Renderable3D::init(const float* vertex, const int& vertexComponentCount, int* layout, const int& layoutComponentCount, const int& stride)
		{
			glGenBuffers(1, &m_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, vertexComponentCount * sizeof(GLfloat), vertex, GL_DYNAMIC_DRAW);
			glGenVertexArrays(1, &m_VAO);
			glBindVertexArray(m_VAO);
			std::sort(layout, layout + layoutComponentCount);
			int offset = 0;
			int attributeComponentCount = 0;
			for (int i = 0; i < layoutComponentCount; i++)
			{
				switch (*layout)
				{
				case POSITION_VALUE: //position
					attributeComponentCount = 3;
					break;
				case UV_VALUE://uv
					attributeComponentCount = 2;
					break;
				case TID_VALUE://tid
					//n += 3;
					break;
				case COLOR_VALUE://color
					attributeComponentCount = 4;
					break;
				case NORMAL_VALUE://normalVec
					attributeComponentCount = 3;
					break;
				}
				glEnableVertexAttribArray(*layout);
				glVertexAttribPointer(*layout, attributeComponentCount, GL_FLOAT, GL_FALSE, stride, (void*)(offset * sizeof(float)));
				switch (*layout)
				{
				case POSITION_VALUE:
					offset += 3;
					break;
				case UV_VALUE:
					offset += 2;
					break;
				case TID_VALUE:
					//offset += 3;
					break;
				case COLOR_VALUE:
					offset += 4;
					break;
				case NORMAL_VALUE:
					offset += 3;
					break;
				}
				layout++;
			}
			unbindVAO();
			unbindVBO();
		}
	}
}