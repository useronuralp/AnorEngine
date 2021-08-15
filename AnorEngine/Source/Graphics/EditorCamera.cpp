#include "pch.h"
#include "Events/EventHandler.h"
#include "EditorCamera.h"


#include "Core/EngineKeyCodes.h"
#include "Core/MouseButtonCodes.h"


#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>
namespace AnorEngine
{
	namespace Graphics
	{
		EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
			:m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
		{
			UpdateView();
		}
		void EditorCamera::OnUpdate(float deltaTime, bool wantToCaptureEvents)
		{
			if (wantToCaptureEvents)
			{
				auto window = Application::Get().GetOpenGLWindow()->GetNativeWindow();
				if (Input::EventHandler::IsKeyDown(ANOR_KEY_LEFT_CONTROL))
				{
					const glm::vec2& mouse{ Input::EventHandler::GetMouseXOffset(window), Input::EventHandler::GetMouseYOffset(window) };
					glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
					m_InitialMousePosition = mouse;

					if (Input::EventHandler::IsMousePressed(ANOR_MOUSE_BUTTON_MIDDLE))
						MousePan(delta);
					else if (Input::EventHandler::IsMousePressed(ANOR_MOUSE_BUTTON_LEFT))
						MouseRotate(delta);
					else if (Input::EventHandler::IsMousePressed(ANOR_MOUSE_BUTTON_RIGHT))
						MouseZoom(delta.y);
				}
			}
			UpdateView();
		}
		void EditorCamera::OnEvent(Ref<Input::Event> ev)
		{
			if (ev->GetEventType() == Input::EventType::MouseScrollEvent)
			{
				auto castEvent = std::static_pointer_cast<Input::MouseScrollEvent>(ev);
				OnMouseScroll(castEvent);
			}
		}
		void EditorCamera::UpdateProjection()
		{
			m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
			m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
		}
		void EditorCamera::UpdateView()
		{
			// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
			m_Position = CalculatePosition();

			glm::quat orientation = GetOrientation();
			m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
			m_ViewMatrix = glm::inverse(m_ViewMatrix);
		}
		bool EditorCamera::OnMouseScroll(Ref<Input::MouseScrollEvent>& ev)
		{
			float delta = ev->GetYOffset() * 0.1f;
			MouseZoom(delta);
			UpdateView();
			return false;
		}
		void EditorCamera::MousePan(const glm::vec2& delta)
		{
			auto [xSpeed, ySpeed] = PanSpeed();
			m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
			m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
		}
		void EditorCamera::MouseRotate(const glm::vec2& delta)
		{
			float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
			m_Yaw += yawSign * delta.x * RotationSpeed();
			m_Pitch += delta.y * RotationSpeed();
		}
		void EditorCamera::MouseZoom(float delta)
		{
			m_Distance -= delta * ZoomSpeed();
			if (m_Distance < 1.0f)
			{
				m_FocalPoint += GetForwardDirection();
				m_Distance = 1.0f;
			}
		}
		std::pair<float, float> EditorCamera::PanSpeed() const
		{
			float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
			float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

			float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
			float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

			return { xFactor, yFactor };
		}
		float EditorCamera::ZoomSpeed() const
		{
			float distance = m_Distance * 0.2f;
			distance = std::max(distance, 0.0f);
			float speed = distance * distance;
			speed = std::min(speed, 100.0f); // max speed = 100
			return speed;
		}

		float EditorCamera::RotationSpeed() const
		{
			return 0.8f;
		}
		glm::vec3 EditorCamera::GetUpDirection() const
		{
			return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		glm::vec3 EditorCamera::GetRightDirection() const
		{
			return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		glm::vec3 EditorCamera::GetForwardDirection() const
		{
			return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
		}
		glm::quat EditorCamera::GetOrientation() const
		{
			return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
		}
		glm::vec3 EditorCamera::CalculatePosition() const
		{
			return m_FocalPoint - GetForwardDirection() * m_Distance;
		}
	}
}