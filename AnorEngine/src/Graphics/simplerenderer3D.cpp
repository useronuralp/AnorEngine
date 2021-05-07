#include "pch.h"
#include "simplerenderer3D.h"
#include "Camera.h"

namespace AnorEngine {
	namespace Graphics {
		void Renderer3D::instancedDraw(Renderable3D& object, Shader& shader, const glm::mat4& camera, const int& vertexCount, const int& repeatAmount)
		{
			//shader.enable();
			//object.bindVAO();
			//
			//updateMVP(object, shader, camera);
			//glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, repeatAmount);
			//
			//object.unbindVAO();
			//shader.disable();
		}
		void Renderer3D::singleDraw(Renderable3D& object, Shader& shader, PerspectiveCamera* camera, const int& vertexCount)
		{
			shader.enable();
			object.bindVAO();

			updateMVP(object, shader, camera);
			glDrawArrays(GL_TRIANGLES, 0 ,vertexCount);

			object.unbindVAO();
			shader.disable();
		}
		void Renderer3D::updateMVP(Renderable3D& object, Shader& shader, PerspectiveCamera* camera)
		{
			m_ShaderModelLocation = glGetUniformLocation(shader.getShaderID(), "ml_matrix");
			m_ShaderViewLocation = glGetUniformLocation(shader.getShaderID(), "vw_matrix");
			m_ShaderProjectionLocation = glGetUniformLocation(shader.getShaderID(), "pr_matrix");
			glUniformMatrix4fv(m_ShaderModelLocation, 1, GL_FALSE, glm::value_ptr(object.m_ModelMatrix));
			glUniformMatrix4fv(m_ShaderViewLocation, 1, GL_FALSE, glm::value_ptr(glm::lookAt(camera->cameraPos, camera->cameraPos + camera->cameraFront, camera->cameraUp)));
			glUniformMatrix4fv(m_ShaderProjectionLocation, 1, GL_FALSE, glm::value_ptr(camera->m_ProjectionMatrix));
		}
	}
}