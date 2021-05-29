#pragma once
#include "Graphics/Buffers/VertexArray.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
namespace AnorEngine
{
	namespace Graphics
	{
		class ANOR_API Renderer2D
		{
		public:
			static void Init();
			static void ClearColor(const glm::vec4& color);
			static void Clear();
			static void Shutdown();
			static void DrawPrimitive(const Ref<VertexArray> vertexArray, const Ref<Shader> shader, const glm::mat4& modelMatrix, const glm::vec4& color);

			static void BeginScene(const Ref<OrthographicCamera> camera);
			static void EndScene();
		private:
			static Ref<OrthographicCamera> s_OrthoCamera;
		};
	}
}