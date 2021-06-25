#pragma once
#include <Events/EventHandler.h>
#include "Camera.h"
namespace AnorEngine
{
	namespace Graphics
	{
		class ANOR_API OrthographicCameraController
		{
		public:
			OrthographicCameraController(Ref<OrthographicCamera> camera, float aspectRatio);
			void OnUpdate(float deltaTime);
			void OnEvent(Ref<Input::Event> e);
			float GetAspectRatio() { return m_AspectRatio * m_ZoomLevel; }
		private:
			bool OnResizeEvent(Ref<Input::WindowResizeEvent> e);
			bool OnMouseScrollEvent(Ref<Input::MouseScrollEvent> e);
		private:
			float m_CameraTranslationSpeed = 5.0f;
			float m_AspectRatio;
			float m_ZoomLevel = 5; //This value is determined by the multiplier in the sandbox.
			glm::vec3 m_CameraPos = {0.0f, 0.0f, 0.0f};
			Ref<OrthographicCamera> m_Camera;
		};
	}
}