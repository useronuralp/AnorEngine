#include "pch.h"
#include "Renderer.h"

namespace AnorEngine
{
	namespace Graphics
	{
		Ref<OrthographicCamera> Renderer::s_OrthoCamera;
		void Renderer::Init()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		void Renderer::BeginScene(Ref<OrthographicCamera> camera)
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

		void Renderer::Submit(Ref<VertexArray> vertexArray, Ref<Shader> shader, const glm::mat4& modelMatrix, const glm::vec4& color)
		{
			shader->enable();
			vertexArray->bind();
			shader->UploadMat4("u_ViewProjMat", s_OrthoCamera->GetViewProjectionMatrix());
			shader->UploadMat4("u_ModelMatrix", modelMatrix);
			shader->UploadFloat4("u_Color", color);
			glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, NULL);
			vertexArray->unbind();
			shader->disable();
		}
	}
}
