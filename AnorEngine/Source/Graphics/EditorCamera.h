#pragma once
#include "Camera.h"
#include "Events/Event.h"

namespace AnorEngine 
{
	namespace Graphics
	{
		//This camera is 3D because of its nature. 2D games will also use this camera.
		class ANOR_API EditorCamera : public Camera
		{
		public:
			EditorCamera() = default;
			EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

			void OnUpdate(float deltaTime, bool wantToCaptureEvents = true);
			void OnEvent(Ref<Input::Event> ev);

			inline float GetDistance() const { return m_Distance; }
			inline void SetDistance(float distance) { m_Distance = distance; }

			inline void SetViewportSize(float width, float height) { m_ViewportWidth = width, m_ViewportHeight = height; UpdateProjection(); }

			glm::vec3 GetUpDirection() const;
			glm::vec3 GetRightDirection() const;
			glm::vec3 GetForwardDirection() const;
			glm::quat GetOrientation() const;
			const glm::vec3& GetPosition() const { return m_Position; }

			float GetPitch() { return m_Pitch; }
			float GetYaw() { return m_Yaw; }

		private:
			void UpdateProjection();
			void UpdateView();

			bool OnMouseScroll(Ref<Input::MouseScrollEvent>& ev);

			void MousePan(const glm::vec2& delta);
			void MouseRotate(const glm::vec2& delta);
			void MouseZoom(float delta);

			glm::vec3 CalculatePosition() const;

			std::pair<float, float> PanSpeed() const;
			float RotationSpeed() const;
			float ZoomSpeed() const;
		private:
			float m_FOV = 45.0f;
			float m_AspectRatio = 1.778f;
			float m_NearClip = 0.1f;
			float m_FarClip = 1000.0f;

			glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
			glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

			float m_Pitch = 0.0f;
			float m_Yaw = 0.0f;

			glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

			float m_Distance = 10.0f;
			float m_ViewportWidth = 1280;
			float m_ViewportHeight = 720;
		};
	}
}