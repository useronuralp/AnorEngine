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
			static void BeginScene(OrthographicCamera* camera);
			static void EndScene();
			static void ClearColor(const glm::vec4& color);
			static void Clear();
			static void Submit(std::shared_ptr<VertexArray>& vertexArray, Shader* shader);
		public:
			static OrthographicCamera* s_OrthoCamera;
		};
	}
}
