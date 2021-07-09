#pragma once
#include "Graphics/Camera.h"

namespace AnorEngine
{
	class ANOR_API SceneCamera : public Graphics::Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;
		void SetOrthographic(float size, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);
		float GetOrhographicSize() { return m_OrthographicSize; }
	private:
		void RecalculateProjectionMatrix();
	private:
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;

		// width / height;
		float m_AspectRatio = 1.0f;
	};
}