#pragma once
#include <glm.hpp>
#include <Graphics/Buffers/VertexArray.h>
#include "Graphics/Camera.h"
#include "Graphics/shader.h"
//#include "RendererCommand.h"
namespace AnorEngine
{
	namespace Graphics
	{
		class ANOR_API Renderer
		{
		public:
			static void BeginScene(Ref<OrthographicCamera> camera);
			static void EndScene();
			static void ClearColor(const glm::vec4& color);
			static void Clear();
			static void Submit(Ref<VertexArray> vertexArray, Ref<Shader> shader, const glm::mat4& modelMatrix, const glm::vec4& color);
		public:
			static Ref<OrthographicCamera> s_OrthoCamera;
		};
	}
}
