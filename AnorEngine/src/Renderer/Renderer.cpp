#include "pch.h"
#include "Renderer.h"

namespace AnorEngine
{
	namespace Graphics
	{
		OrthographicCamera* Renderer::s_OrthoCamera;
		void Renderer::BeginScene(OrthographicCamera* camera)
		{
			Renderer::s_OrthoCamera = camera;
		}
		void Renderer::EndScene()
		{
		}

		void Renderer::ClearColor(const glm::vec4& color)
		{
			glClearColor(color.r, color.g, color.b, color.a);
		}

		void Renderer::Clear()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void Renderer::Submit(std::shared_ptr<VertexArray>& vertexArray, Shader* shader)
		{
			shader->enable();
			vertexArray->bind();
			shader->UploadMat4("u_viewprojMat", s_OrthoCamera->GetViewProjectionMatrix());
			glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, NULL);
			vertexArray->unbind();
			shader->disable();
		}
	}
}
