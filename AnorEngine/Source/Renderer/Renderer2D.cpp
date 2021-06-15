#include "pch.h"
#include "Renderer2D.h"
namespace AnorEngine
{
	namespace Graphics
	{

		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			//glm::vec3 Normals;
		};
		struct Renderer2DData
		{
			const uint32_t MaxQuads = 10000;
			const uint32_t MaxVertices = MaxQuads * 4;
			const uint32_t MaxIndices = MaxQuads * 6;

			Ref<VertexArray> QuadVertexArray;
			Ref<VertexBuffer> QuadVertexBuffer;
			Ref<Shader> QuadShader;
			Ref<Texture> QuadTexture;

			uint32_t QuadIndexCount = 0;

			QuadVertex* QuadVertexBufferBase = nullptr;
			QuadVertex* QuadVertexBufferPtr = nullptr;
		};


		static Renderer2DData s_Data;
		Ref<OrthographicCamera> Renderer2D::s_OrthoCamera;

		void Renderer2D::Init()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			s_Data.QuadVertexArray = std::make_shared<VertexArray>();
			BufferLayout QuadLayout = { {ShaderDataType::vec3, "a_Position", 0} ,{ShaderDataType::vec4, "a_Color", 1} , {ShaderDataType::vec2, "a_TexCoord", 2} };
			s_Data.QuadVertexBuffer = std::make_shared<VertexBuffer>(s_Data.MaxQuads * sizeof(QuadVertex), QuadLayout);
			s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
			s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices]; // Set the address of the base when initializing the renderer.

			std::string solutionDir = __SOLUTION_DIR;
			ShaderLibrary::LoadShader("2DShader", solutionDir + "AnorEngine\\Assets\\Shaders\\2DShader.shader");
			ShaderLibrary::LoadShader("TextureShader", solutionDir + "AnorEngine\\Assets\\Shaders\\2DTextureShader.shader");
			ShaderLibrary::LoadShader("2DBackgroundShader", solutionDir + "AnorEngine\\Assets\\Shaders\\2DBackgroundShader.shader");
			s_Data.QuadShader = ShaderLibrary::GetShader("2DShader");

			uint32_t* QuadIndices = new uint32_t[s_Data.MaxIndices];

			uint32_t offset = 0;
			for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
			{
				QuadIndices[i + 0] = offset + 0;
				QuadIndices[i + 1] = offset + 1;
				QuadIndices[i + 2] = offset + 2;

				QuadIndices[i + 3] = offset + 2;
				QuadIndices[i + 4] = offset + 3;
				QuadIndices[i + 5] = offset + 0;

				offset += 4;
			}
			s_Data.QuadVertexArray->SetIndexBuffer(std::make_shared<IndexBuffer>(QuadIndices, s_Data.MaxIndices));
			delete[] QuadIndices;
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
			vertexArray->Bind();
			if (shader->GetName() == "2DBackgroundShader")
			{
				shader->UploadMat4("u_ViewProjMat", s_OrthoCamera->GetBackgroundViewProjectionMatrix());
			}
			else
			{
				shader->UploadMat4("u_ViewProjMat", s_OrthoCamera->GetViewProjectionMatrix());
			}
			shader->UploadMat4("u_ModelMatrix", modelMatrix);
			shader->UploadFloat4("u_Color", color);
			if(texture != nullptr)
				texture->Bind();
			glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, NULL);
			if(texture != nullptr)
				texture->Unbind();
			vertexArray->Unbind();
			shader->disable();
		}
		void Renderer2D::Submit(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
		{
			s_Data.QuadVertexBufferPtr->Position = position;
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f};
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadIndexCount += 6;
			
		}

		void Renderer2D::Flush()
		{
			uint32_t count;
			count = s_Data.QuadIndexCount ? s_Data.QuadIndexCount : 0;

			s_Data.QuadShader->enable();
			s_Data.QuadVertexArray->Bind();
			s_Data.QuadShader->UploadMat4("u_ViewProjMat", s_OrthoCamera->GetViewProjectionMatrix());
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
			s_Data.QuadVertexArray->Unbind();
			s_Data.QuadShader->disable();
		}

		void Renderer2D::BeginScene(const Ref<OrthographicCamera> camera)
		{
			s_OrthoCamera = camera;
			s_Data.QuadIndexCount = 0;
			s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		}

		void Renderer2D::EndScene()
		{
			uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
			Flush();
		}
	}
}
