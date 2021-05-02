#include "pch.h"
#include "simplerenderer3D.h"


namespace GameEngineTest {
	namespace Graphics {
		void Renderer3D::instancedDraw(Renderable3D& object, Shader& shader, const glm::mat4& camera, const int& vertexCount, const int& repeatAmount)
		{
			shader.enable();
			object.bindVAO();

			updateMVP(object, shader, camera);
			glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, repeatAmount);

			object.unbindVAO();
			shader.disable();
		}
		void Renderer3D::singleDraw(Renderable3D& object, Shader& shader, const glm::mat4& camera, const int& vertexCount)
		{
			shader.enable();
			object.bindVAO();

			updateMVP(object, shader, camera);
			glDrawArrays(GL_TRIANGLES, 0 ,vertexCount);

			//object.unbindVAO();
			//shader.disable();
		}
		void Renderer3D::updateMVP(Renderable3D& object, Shader& shader, const glm::mat4& camera)
		{
			object.viewMatrix = camera;
			modelLocation = glGetUniformLocation(shader.getShaderID(), "ml_matrix");
			viewLocation = glGetUniformLocation(shader.getShaderID(), "vw_matrix");
			projectionLocation = glGetUniformLocation(shader.getShaderID(), "pr_matrix");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(object.modelMatrix));
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(object.viewMatrix));
			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(object.projectionMatrix));
		}
	}
}