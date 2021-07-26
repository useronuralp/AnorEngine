#pragma once
#include "Graphics/Camera.h"

namespace AnorEngine
{
	enum class ANOR_API ProjectionType
	{
		Orthographic = 0, Perspective = 1
	};
	class ANOR_API SceneCamera : public Graphics::Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;
		void SetViewportSize(uint32_t width, uint32_t height);
		ProjectionType GetProjectionType() { return m_ProjectionType; }

		void SetNearClipPerspective(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjectionMatrix(); }
		void SetFarClipPerspective(float farClip) { m_PerspectiveFar = farClip; RecalculateProjectionMatrix(); }
		void SetPerspectiveFOV(float FOV) { m_FOV = FOV; RecalculateProjectionMatrix(); }
		float GetPerspectiveFOV() { return m_FOV; }
		float GetNearClipPerspective() { return m_PerspectiveNear; }
		float GetFarClipPerspective() { return m_PerspectiveFar; }

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetNearClipOrthographic(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjectionMatrix(); }
		void SetFarClipOrthographic(float farClip) {m_OrthographicFar = farClip; RecalculateProjectionMatrix(); }
		float GetNearClipOrthographic() { return m_OrthographicNear; }
		float GetFarClipOrthographic() { return m_OrthographicFar; }
		void SetOrthoGraphicSize(float size) { m_OrthographicSize = size; RecalculateProjectionMatrix(); }
		float GetOrhographicSize() { return m_OrthographicSize; }


	private:
		void RecalculateProjectionMatrix();
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_FOV = 45.0f;
		float m_PerspectiveNear = 0.01f;
		float m_PerspectiveFar = 10000;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;

		float m_AspectRatio = 1.0f;
	};
}