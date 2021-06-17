#include "pch.h"
#include "Renderer2D.h"
namespace AnorEngine
{
	namespace Graphics
	{

		struct QuadVertex //Attributes of each vertex. If you want to extend the attributes, you have to add it in here and extend the BufferLayout as well.
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float	  TexIndex;
			//glm::vec3 Normals;
		};
		struct Renderer2DData //Used to store data in this static class. Defined this struct in the .cpp so that nothing else can include it.
		{
			const uint32_t			 MaxQuads = 10000;
			const uint32_t			 MaxVertices = MaxQuads * 4;
			const uint32_t			 MaxIndices = MaxQuads * 6;
			static const uint32_t	 MaxTextureSlots = 32;
									 
			Ref<VertexArray>		 QuadVertexArray;
			Ref<VertexBuffer>		 QuadVertexBuffer;
			Ref<Shader>				 QuadShader;
			Ref<Texture>			 WhiteTexture;
			Ref<Texture>			 QuadTexture;
									 
			uint32_t				 QuadIndexCount = 0;
									 
			QuadVertex*				 QuadVertexBufferBase = nullptr;
			QuadVertex*				 QuadVertexBufferPtr = nullptr;

			std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
			uint32_t				 TextureSlotIndex = 1; // 0 = white texture;
		};


		static Renderer2DData s_Data;
		Ref<OrthographicCamera> Renderer2D::s_OrthoCamera;

		void Renderer2D::Init()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			BufferLayout QuadBufferLayout = { {ShaderDataType::vec3, "a_Position", 0} ,{ShaderDataType::vec4, "a_Color", 1} , {ShaderDataType::vec2, "a_TexCoord", 2} , {ShaderDataType::vec, "a_TexIndex", 3} };
			s_Data.QuadVertexArray = std::make_shared<VertexArray>();
			s_Data.QuadVertexBuffer = std::make_shared<VertexBuffer>(s_Data.MaxQuads * sizeof(QuadVertex), QuadBufferLayout);
			s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
			s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices]; // Set the address of the base when initializing the renderer.

			std::string solutionDir = __SOLUTION_DIR;
			ShaderLibrary::LoadShader("2DShader", solutionDir + "AnorEngine\\Assets\\Shaders\\2DShader.shader");
			ShaderLibrary::LoadShader("2DBackgroundShader", solutionDir + "AnorEngine\\Assets\\Shaders\\2DBackgroundShader.shader");
			ShaderLibrary::LoadShader("TextureShader", solutionDir + "AnorEngine\\Assets\\Shaders\\2DTextureShader.shader");
			s_Data.QuadShader = ShaderLibrary::GetShader("2DShader");

			int samplers[s_Data.MaxTextureSlots];
			for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			{
				samplers[i] = i;
			}
			s_Data.QuadShader->UploadIntegerArray("u_Textures", samplers, s_Data.MaxTextureSlots);

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


			//Creating and adding a white texture. This is basically used when we only want to render a quad with its color. In that case, we multiply the color values with this white texture meaning 1. Therefore 
			//getting the original colors on the screen.
			s_Data.WhiteTexture = std::make_shared<Texture>(solutionDir + "AnorEngine\\Assets\\Textures\\WhiteTexture.PNG"); //TODO: Create this texture without a using a PNG.
			s_Data.TextureSlots[0] = s_Data.WhiteTexture;
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
				texture->Bind(0);
			glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, NULL);
			if(texture != nullptr)
				texture->Unbind();
			vertexArray->Unbind();
			shader->disable();
		}
		void Renderer2D::Submit(const glm::vec3& position, const glm::vec2& size, const Ref<Texture> texture, const glm::vec4& color)
		{

			float textureIndex = 0.0f;
			//Setting the textureIndex to its proper value before using it. If we can't find a value for it, it should remain 0.0f.
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i].get() == *texture.get())
				{
					textureIndex = (float)i;
					break;
				}
			}
			//Using the processed textureIndex
			if (textureIndex == 0.0f)
			{
				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
				s_Data.TextureSlotIndex++;
			}

			//If you don't pass a color, the default value is white.
			s_Data.QuadVertexBufferPtr->Position = position;
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadIndexCount += 6;
		}
		//This Submit function is for when you don't want to submit a texture and draw using only a color.
		void Renderer2D::Submit(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
		{
			s_Data.QuadVertexBufferPtr->Position = position;
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
			s_Data.QuadVertexBufferPtr->TexIndex = 0.0f; //White texture
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f};
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
			s_Data.QuadVertexBufferPtr->TexIndex = 0.0f;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
			s_Data.QuadVertexBufferPtr->TexIndex = 0.0f;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
			s_Data.QuadVertexBufferPtr->TexIndex = 0.0f;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadIndexCount += 6;
			
		}

		void Renderer2D::Flush()
		{
			uint32_t count;
			count = s_Data.QuadIndexCount ? s_Data.QuadIndexCount : 0;

			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			{
				s_Data.TextureSlots[i]->Bind(i);
			}

			s_Data.QuadVertexArray->Bind();
			s_Data.QuadShader->enable();
			//Grabbing the camera View Projection matrix from here. This is a must.
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
			s_Data.TextureSlotIndex = 1;
		}

		void Renderer2D::EndScene()
		{
			uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
			Flush();
		}
	}
}
