#include "pch.h"
#include "OrthographicCameraController.h"
#include "Layers/Layer.h"
#include "Events/Event.h"
#include "Core/EngineKeyCodes.h"
#include "Core/MouseButtonCodes.h"
namespace AnorEngine {
	namespace Graphics
	{
		OrthographicCameraController::OrthographicCameraController(Ref<OrthographicCamera> camera, float aspectRatio)
			:m_OrthographicCamera(camera), m_AspectRatio(aspectRatio), m_Bounds({ -aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel })
		{
		}
		void OrthographicCameraController::OnUpdate(float deltaTime)
		{

			m_CameraTranslationSpeed = m_ZoomLevel; //liner curve to smooth out the translation a little bit
			if (Input::EventHandler::IsKeyDown(ANOR_KEY_W))
			{
				m_CameraPos.y += m_CameraTranslationSpeed * deltaTime;
			}
			else if (Input::EventHandler::IsKeyDown(ANOR_KEY_S))
			{
				m_CameraPos.y -= m_CameraTranslationSpeed * deltaTime;
			}
			if (Input::EventHandler::IsKeyDown(ANOR_KEY_A))
			{
				m_CameraPos.x -= m_CameraTranslationSpeed * deltaTime;
			}
			else if (Input::EventHandler::IsKeyDown(ANOR_KEY_D))
			{
				m_CameraPos.x += m_CameraTranslationSpeed * deltaTime;
			}
			m_OrthographicCamera->SetPosition(m_CameraPos);
		}

		void OrthographicCameraController::OnEvent(Ref<Input::Event> e)
		{
			if (e->GetEventType() == Input::EventType::WindowResizeEvent)
			{
				OnResizeEvent(std::static_pointer_cast<Input::WindowResizeEvent>(e));
			}
			else if (e->GetEventType() == Input::EventType::MouseScrollEvent)
			{
				OnMouseScrollEvent(std::static_pointer_cast<Input::MouseScrollEvent>(e));
			}
		}
		bool OrthographicCameraController::OnResizeEvent(Ref<Input::WindowResizeEvent> e)
		{	
			m_AspectRatio = (float)e->GetWidth() / (float)e->GetHeight();
			m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
			m_OrthographicCamera->SetProjectionMatrix(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
			return false;
		}
		bool OrthographicCameraController::OnMouseScrollEvent(Ref<Input::MouseScrollEvent> e)
		{
			m_ZoomLevel -= e->GetYOffset() * 0.15f;
			m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
			m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
			m_OrthographicCamera->SetProjectionMatrix(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
			return false;
		}
	}
}