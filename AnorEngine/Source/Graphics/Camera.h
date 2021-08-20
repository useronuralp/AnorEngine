#pragma once
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
namespace AnorEngine
{
	namespace Graphics
	{
		class ANOR_API Camera
		{
		public:
			Camera() = default;
			Camera(const glm::mat4& projection)
				:m_ProjectionMatrix(projection) {}
			Camera(const Camera* camera)
				:m_ProjectionMatrix(camera->m_ProjectionMatrix) {}
			virtual ~Camera() = default;
		public:
			void SetProjectionMatrix(float left, float right, float bottom, float top);
			const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
			const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
			const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		protected:
			glm::mat4 m_ProjectionMatrix{ 1.0f };
			glm::mat4 m_ViewMatrix{ 1.0f };
			glm::mat4 m_ViewProjectionMatrix{ 1.0f }; //Combination of the upper two matrices.
		};

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

			glm::vec3 cameraPos;
			glm::vec3 cameraFront;
			glm::vec3 cameraUp;

			glm::mat4 m_ProjectionMatrix;
			glm::mat4 m_ViewMatrix;
			PerspectiveCamera(float height, float weight);
			double GetRenderTime() { return glfwGetTime(); }
			void OnUpdate();
		};

		class ANOR_API OrthographicCamera
		{
		public:
			OrthographicCamera(float left, float right, float bottom, float top);
			const glm::vec3& GetPosition() const { return m_Position; }
			const float& GetRotation() const { return m_Rotation; }
			void SetProjectionMatrix(float left, float right, float bottom, float top);
			void SetPosition(glm::vec3 position) { m_Position = position; RecalculateViewProjectionMatrix(); }
			void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewProjectionMatrix(); }


			const glm::mat4& GetBackgroundViewProjectionMatrix() { return m_BackgroundViewProjMatrix; }
			const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
			const glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
			const glm::mat4& GetViewProjectionMatrix() { return m_ViewProjectionMatrix; }

			double GetRenderTime() { return glfwGetTime(); }
		private:
			void RecalculateViewProjectionMatrix();
		private:
			glm::mat4 m_ProjectionMatrix;
			glm::mat4 m_ViewMatrix;
			glm::mat4 m_ViewProjectionMatrix;
			glm::mat4 m_BackgroundViewProjMatrix;


			glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
			float m_Rotation = 0.0f;
		};
	}
}