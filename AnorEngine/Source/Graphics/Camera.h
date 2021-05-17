#pragma once
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
namespace AnorEngine
{
	namespace Graphics
	{
		class ANOR_API PerspectiveCamera
		{
		public:
			float deltaTime = 0.0f;	// Time between current frame and last frame
			float lastFrame = 0.0f; // Time of last frame
			float m_Height;
			float m_Width;
			float lastY;
			float lastX;
			float YAW;
			float PITCH;
			float sensitivity;

			glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 5.0f);
			glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

			glm::mat4 m_ProjectionMatrix = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.01f, 1000.0f);
			glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
			PerspectiveCamera(float height, float weight);
			int GetRenderTime() { return glfwGetTime(); }
			void OnUpdate();
		};

		class ANOR_API OrthographicCamera
		{
		public:
			OrthographicCamera(float left, float right, float bottom, float top);
			const glm::vec3& GetPosition() const { return m_Position; }
			const float& GetRotation() const { return m_Rotation; }
			void SetPosition(glm::vec3 position) { m_Position = position; RecalculateViewMatrix(); }
			void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

			const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
			const glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
			const glm::mat4& GetViewProjectionMatrix() { return m_ViewProjectionMatrix; }

			int GetRenderTime() { return glfwGetTime(); }
		private:
			void RecalculateViewMatrix();
		private:
			glm::mat4 m_ProjectionMatrix;
			glm::mat4 m_ViewMatrix;
			glm::mat4 m_ViewProjectionMatrix;

			glm::vec3 m_Position = { 0.0f,0.0f,0.0f };
			float m_Rotation = 0.0f;
		};
	}
}