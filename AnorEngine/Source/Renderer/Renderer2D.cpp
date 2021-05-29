#include "pch.h"
#include "Renderer2D.h"
namespace AnorEngine
{
	namespace Graphics
	{
		Ref<OrthographicCamera> Renderer2D::s_OrthoCamera;
		void Renderer2D::Init()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		void Renderer2D::Shutdown()
		{
		}

		void Renderer2D::ClearColor(const glm::vec4& color)
		{
			glClearColor(color.r, color.g, color.b, color.a);
		}

		void Renderer2D::Clear()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		void Renderer2D::DrawPrimitive(const Ref<VertexArray> vertexArray, const Ref<Shader> shader, const glm::mat4& modelMatrix, const glm::vec4& color, const Ref<Texture> texture)
		{
			shader->enable();
			vertexArray->bind();
			shader->UploadMat4("u_ViewProjMat", s_OrthoCamera->GetViewProjectionMatrix());
			shader->UploadMat4("u_ModelMatrix", modelMatrix);
			shader->UploadFloat4("u_Color", color);
			if(texture != nullptr)
				texture->Bind();
			glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, NULL);
			if(texture != nullptr)
				texture->Unbind();
			vertexArray->unbind();
			shader->disable();
		}	
		void Renderer2D::BeginScene(const Ref<OrthographicCamera> camera)
		{
			s_OrthoCamera = camera;
		}

		void Renderer2D::EndScene()
		{
		}
	}
}
