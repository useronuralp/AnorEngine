#include "pch.h"
#include "Camera.h"
namespace AnorEngine
{
	namespace Graphics
	{
		PerspectiveCamera::PerspectiveCamera(float height, float width)
			:m_Height(height), m_Width(width), m_ProjectionMatrix(glm::perspective(-1280.0f / 720.0f * (5), 1280.0f / 720.0f * (5), -1.0f * (5), 1.0f * (5)))
		{
			deltaTime = 0.0f;
			lastFrame = 0.0f;
			lastY = m_Height / 2.0f;
			lastX = m_Width / 2.0f;
			YAW = -90.0f;
			PITCH = 0.0f;
			sensitivity = 0.05f;
			OnUpdate();

			cameraPos = glm::vec3(0.0f, 2.0f, 5.0f);
			cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
			cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		}
		void PerspectiveCamera::OnUpdate()
		{
			glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		}

		OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
			:m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(glm::mat4(1.0f))
		{
			m_BackgroundViewProjMatrix = m_ProjectionMatrix * m_ViewMatrix;
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
		void OrthographicCamera::SetProjectionMatrix(float left, float right, float bottom, float top)
		{
			m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
		void OrthographicCamera::RecalculateViewMatrix()
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
			m_ViewMatrix = glm::inverse(transform);
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
	}
}