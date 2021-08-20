#include "pch.h"
#include "Renderer2D.h"
#include "Scene/Components.h"
namespace AnorEngine
{
	namespace Graphics
	{
		struct QuadVertex //Attributes of each vertex. If you want to extend the attributes, you have to add it in here and extend the BufferLayout accordingly as well.
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float	  TexIndex;
			uint32_t  EntityID;
			//glm::vec3 Normals;
		};
		struct Renderer2DData //Used to store data in this static class. Defined this struct in the .cpp so that nothing else can include it.
		{
			static const uint32_t					  MaxQuads = 10000;
			static const uint32_t					  MaxVertices = MaxQuads * 4;
			static const uint32_t					  MaxIndices = MaxQuads * 6;
			static const uint32_t					  MaxTextureSlots = 32;
													  
			Ref<VertexArray>						  QuadVertexArray;
			Ref<VertexBuffer>						  QuadVertexBuffer;
			Ref<Shader>								  QuadShader;
			Ref<Texture>							  WhiteTexture;
			Ref<Texture>							  QuadTexture;
													  
			uint32_t								  QuadIndexCount = 0;
													  
			QuadVertex*								  QuadVertexBufferBase = nullptr;
			QuadVertex*								  QuadVertexBufferPtr = nullptr;

			std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
			uint32_t								  TextureSlotIndex = 1; // 0 = white texture;

			glm::vec4								  QuadVertexPositions[4] = {};
			uint32_t								  NumberOfDrawCalls = 0;

			//Global cameras
			Ref<EditorCamera>						  EditorCamera;
			Ref<OrthographicCamera>					  OrthographicCamera;
			Camera*									  RuntimeCamera;

			//Experimenatal 3D stuff
			Ref<VertexArray>						  CubeVertexArray;
			Ref<VertexBuffer>						  CubeVertexBuffer;
			Ref<CubeMapTexture>						  SkyboxTexture;
			int										  PointLightCount = 0;

		};

		//Instance the s_Data here for only once.
		static Renderer2DData s_Data;

		void Renderer2D::Init()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_DEPTH_TEST);
			

			float skyboxVertices[] = {
				// positions          
				-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
				 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
				 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
				 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
				-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

				-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
				 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
				 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
				 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

				-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
				-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
				-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
				-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
				-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
				-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

				 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
				 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
				 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
				 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

				-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
				 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
				 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
				 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
				-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
			};
			
			BufferLayout CubeBufferLayout = {
				{ShaderDataType::vec3, "a_Position", 0},
				{ShaderDataType::vec3, "a_Normal", 1},
				{ShaderDataType::vec2, "a_UV", 2}
			};
			s_Data.CubeVertexArray = std::make_shared<VertexArray>();
			s_Data.CubeVertexBuffer = std::make_shared<VertexBuffer>(skyboxVertices, 288 * sizeof(float), CubeBufferLayout);
			s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);

			BufferLayout QuadBufferLayout = {
				{ShaderDataType::vec3, "a_Position", 0} ,
				{ShaderDataType::vec4, "a_Color",    1} , 
				{ShaderDataType::vec2, "a_TexCoord", 2} , 
				{ShaderDataType::vec, "a_TexIndex",  3} , 
				{ShaderDataType::Int, "a_EntityID",  4} 
			};

			s_Data.QuadVertexArray = std::make_shared<VertexArray>();
			s_Data.QuadVertexBuffer = std::make_shared<VertexBuffer>(Renderer2DData::MaxQuads * sizeof(QuadVertex), QuadBufferLayout);
			s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
			s_Data.QuadVertexBufferBase = new QuadVertex[Renderer2DData::MaxVertices]; // Set the address of the base when initializing the renderer.

			
			//Loading the shaders into the ShaderLibrary. You can access all of the shaders with an iterator in a for loop.
			ShaderLibrary::LoadShader("SkyboxShader", std::string(__SOLUTION_DIR) + "AnorEngine\\Assets\\Shaders\\CubemapShader.shader");
			ShaderLibrary::LoadShader("CubeShader", std::string(__SOLUTION_DIR) + "AnorEngine\\Assets\\Shaders\\CubeShader.shader");
			ShaderLibrary::LoadShader("LightCubeShader", std::string(__SOLUTION_DIR) + "AnorEngine\\Assets\\Shaders\\LightCubeShader.shader");
			ShaderLibrary::LoadShader("2DShader", std::string(__SOLUTION_DIR) + "AnorEngine\\Assets\\Shaders\\2DShader.shader");

			int samplers[s_Data.MaxTextureSlots];
			for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			{
				samplers[i] = i;
			}		
			ShaderLibrary::GetShader("2DShader")->UploadUniform("u_Textures[0]", sizeof(float) * s_Data.MaxTextureSlots, &samplers);

			uint32_t* QuadIndices = new uint32_t[Renderer2DData::MaxIndices];
			uint32_t offset = 0;
			for (uint32_t i = 0; i < Renderer2DData::MaxIndices; i += 6)
			{
				QuadIndices[i + 0] = offset + 0;
				QuadIndices[i + 1] = offset + 1;
				QuadIndices[i + 2] = offset + 2;

				QuadIndices[i + 3] = offset + 2;
				QuadIndices[i + 4] = offset + 3;
				QuadIndices[i + 5] = offset + 0;

				offset += 4;
			}
			s_Data.QuadVertexArray->SetIndexBuffer(std::make_shared<IndexBuffer>(QuadIndices, Renderer2DData::MaxIndices));
			delete[] QuadIndices;


			//Creating and adding a white texture. This is basically used when we only want to render a quad with its color. In that case, we multiply the color values with this white texture meaning 1. Therefore 
			//getting the original colors on the screen.
			s_Data.WhiteTexture = std::make_shared<Texture>(std::string(__SOLUTION_DIR) + "AnorEngine\\Assets\\Textures\\WhiteTexture.PNG"); //TODO: Create this texture without a using a PNG.
			s_Data.TextureSlots[0] = s_Data.WhiteTexture;

			std::vector<std::string> cubeMapFaces
			{
				std::string(__SOLUTION_DIR) + "AnorEngine\\Assets\\Textures\\Skyboxes\\skybox\\left.jpg",
				std::string(__SOLUTION_DIR) + "AnorEngine\\Assets\\Textures\\Skyboxes\\skybox\\right.jpg",
				std::string(__SOLUTION_DIR) + "AnorEngine\\Assets\\Textures\\Skyboxes\\skybox\\top.jpg",
				std::string(__SOLUTION_DIR) + "AnorEngine\\Assets\\Textures\\Skyboxes\\skybox\\bottom.jpg",
				std::string(__SOLUTION_DIR) + "AnorEngine\\Assets\\Textures\\Skyboxes\\skybox\\front.jpg",
				std::string(__SOLUTION_DIR) + "AnorEngine\\Assets\\Textures\\Skyboxes\\skybox\\back.jpg"
			};

			
			s_Data.SkyboxTexture = std::make_shared<CubeMapTexture>(cubeMapFaces);


			//Explicitly defining the vertex positions of the quads here so that we can do transform operations on these in CPU side.
			s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
		}
		void Renderer2D::SetPointLightCount(int count)
		{
			s_Data.PointLightCount = count;
		}
		void Renderer2D::SetPointLightInAllShaders(const TransformComponent& tc, const MeshRendererComponent& mc, const PointLightComponent& plc, int index)
		{
			for (auto& [shaderName, shader] : ShaderLibrary::GetLibrary())
			{
				shader->UploadUniform("u_PointLights[" + std::to_string(index) + "].color", sizeof(mc.Color), &mc.Color);
				shader->UploadUniform("u_PointLights[" + std::to_string(index) + "].position", sizeof(tc.Translation), &tc.Translation);
				shader->UploadUniform("u_PointLights[" + std::to_string(index) + "].constant", sizeof(float), &plc.Constant);
				shader->UploadUniform("u_PointLights[" + std::to_string(index) + "].Linear", sizeof(plc.Linear), &plc.Linear);
				shader->UploadUniform("u_PointLights[" + std::to_string(index) + "].quadratic", sizeof(plc.Quadratic), &plc.Quadratic);
				shader->UploadUniform("u_PointLights[" + std::to_string(index) + "].intensity", sizeof(plc.Intensity), &plc.Intensity);		
				shader->UploadUniform("u_PointLights[" + std::to_string(index) + "].ambient", sizeof(mc.Material.Properties.Ambient), &mc.Material.Properties.Ambient);
				shader->UploadUniform("u_PointLights[" + std::to_string(index) + "].diffuse", sizeof(mc.Material.Properties.Diffuse), &mc.Material.Properties.Diffuse); // darken diffuse light a bit
				shader->UploadUniform("u_PointLights[" + std::to_string(index) + "].specular", sizeof(mc.Material.Properties.Specular), &mc.Material.Properties.Specular);
			}
		}
		void Renderer2D::DrawCube(const TransformComponent& tc, const MeshRendererComponent& mc, const TagComponent& tagc)
		{
			mc.Material.Shader->UploadUniform("u_Sampler", sizeof(mc.Material.Texture->GetTextureID()), &mc.Material.Texture->GetTextureID());
			
			mc.Material.Shader->UploadUniform("u_Transform", sizeof(tc.GetTransform()), &tc.GetTransform());

			mc.Material.Shader->UploadUniform("u_Color", sizeof(mc.Color), &mc.Color);
			mc.Material.Shader->UploadUniform("u_EntityID", sizeof(tc.EntityID), &tc.EntityID);

			mc.Material.Shader->UploadUniform("u_EntityID", sizeof(tc.EntityID), &tc.EntityID);
			mc.Material.Shader->UploadUniform("u_Material.ambient", sizeof(mc.Material.Properties.Ambient), &mc.Material.Properties.Ambient);
			mc.Material.Shader->UploadUniform("u_Material.diffuse", sizeof(mc.Material.Properties.Diffuse), &mc.Material.Properties.Diffuse);
			mc.Material.Shader->UploadUniform("u_Material.specular", sizeof(mc.Material.Properties.Specular), &mc.Material.Properties.Specular);
			mc.Material.Shader->UploadUniform("u_Material.shininess", sizeof(mc.Material.Properties.Shininess), &mc.Material.Properties.Shininess);
			mc.Material.Shader->UploadUniform("u_Material.metalness", sizeof(mc.Material.Properties.Metalness), &mc.Material.Properties.Metalness);

			mc.Material.Texture->Bind(mc.Material.Texture->GetTextureID());
			mc.Material.Shader->Enable();
			s_Data.CubeVertexArray->Bind();

			glDrawArrays(GL_TRIANGLES, 0, 36);

			s_Data.CubeVertexArray->Unbind();
			mc.Material.Shader->Disable();
			mc.Material.Texture->Unbind();
		}
		void Renderer2D::DrawSkybox()
		{
			glDepthMask(GL_FALSE);

			glBindTexture(GL_TEXTURE_CUBE_MAP, s_Data.SkyboxTexture->GetTextureID());
			ShaderLibrary::GetShader("SkyboxShader")->Enable();
			s_Data.CubeVertexArray->Bind();

			glDrawArrays(GL_TRIANGLES, 0, 36);
			glDepthMask(GL_TRUE);

			s_Data.CubeVertexArray->Unbind();
			ShaderLibrary::GetShader("SkyboxShader")->Disable();
		}
		void Renderer2D::Submit(const TransformComponent& tc, SpriteRendererComponent& sc, int entityID)
		{
			if ((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase == Renderer2DData::MaxQuads * sizeof(QuadVertex))
			{
				EndScene();
				s_Data.QuadIndexCount = 0;
				s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
				s_Data.TextureSlotIndex = 1;
			}
			float textureIndex = 0.0f;
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i].get() == *sc.Texture.get())
				{
					textureIndex = (float)i;
					break;
				}
			}
			if (textureIndex == 0.0f)
			{
				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = sc.Texture;
				s_Data.TextureSlotIndex++;
			}

			//This scaling is neccessary to match the sizes of the textures and required quads to draw them.
			glm::mat4 transformEdited = glm::scale(tc.GetTransform(), { sc.TextureSize.x, sc.TextureSize.y, 1.0f });

			s_Data.QuadVertexBufferPtr->Position = transformEdited * s_Data.QuadVertexPositions[0];
			s_Data.QuadVertexBufferPtr->Color = sc.Color;
			s_Data.QuadVertexBufferPtr->TexCoord = { sc.SubTextureOffset.x * (sc.SubTextureDimensions.x / sc.Texture->GetWidth()), sc.SubTextureOffset.y * (sc.SubTextureDimensions.y / sc.Texture->GetHeight()) };
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = transformEdited * s_Data.QuadVertexPositions[1];
			s_Data.QuadVertexBufferPtr->Color = sc.Color;
			s_Data.QuadVertexBufferPtr->TexCoord = { (sc.SubTextureOffset.x + sc.TextureSize.x) * (sc.SubTextureDimensions.x / sc.Texture->GetWidth()), sc.SubTextureOffset.y * (sc.SubTextureDimensions.y / sc.Texture->GetHeight()) };
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = transformEdited * s_Data.QuadVertexPositions[2];
			s_Data.QuadVertexBufferPtr->Color = sc.Color;
			s_Data.QuadVertexBufferPtr->TexCoord = { (sc.SubTextureOffset.x + sc.TextureSize.x) * (sc.SubTextureDimensions.x / sc.Texture->GetWidth()), (sc.SubTextureOffset.y + sc.TextureSize.y) * (sc.SubTextureDimensions.y / sc.Texture->GetHeight()) };
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = transformEdited * s_Data.QuadVertexPositions[3];
			s_Data.QuadVertexBufferPtr->Color = sc.Color;
			s_Data.QuadVertexBufferPtr->TexCoord = { sc.SubTextureOffset.x * (sc.SubTextureDimensions.x / sc.Texture->GetWidth()), (sc.SubTextureOffset.y + sc.TextureSize.y) * (sc.SubTextureDimensions.y / sc.Texture->GetHeight()) };
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadIndexCount += 6;
		}
		void Renderer2D::Flush()
		{
			uint32_t count = s_Data.QuadIndexCount ? s_Data.QuadIndexCount : 0;
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			{
				s_Data.TextureSlots[i]->Bind(i);
			}
			s_Data.NumberOfDrawCalls++;
			s_Data.QuadVertexArray->Bind();
			ShaderLibrary::GetShader("2DShader")->Enable();
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
			s_Data.QuadVertexArray->Unbind();
			ShaderLibrary::GetShader("2DShader")->Disable();
		}
		void Renderer2D::BeginScene(const Ref<EditorCamera>& camera)
		{
			DrawSkybox();
			s_Data.EditorCamera = camera;

			int clearValue = -1;
			//TODO: Hard coded the color attachment index as 4 because it is known at the moment.
			glClearTexImage(4, 0, GL_RED_INTEGER, GL_INT, &clearValue);

			for (auto& [shaderName, shader] : ShaderLibrary::GetLibrary())
			{	
				if (shaderName == "SkyboxShader")
				{
					auto viewProjMatrix = s_Data.EditorCamera->GetProjectionMatrix() * glm::mat4(glm::mat3(s_Data.EditorCamera->GetViewMatrix()));
					shader->UploadUniform("u_ViewProjMat", sizeof(viewProjMatrix), &(viewProjMatrix));
				}
				else
				{
					shader->UploadUniform("u_ViewProjMat", sizeof(s_Data.EditorCamera->GetViewProjectionMatrix()), &s_Data.EditorCamera->GetViewProjectionMatrix());
					shader->UploadUniform("u_CameraPos", sizeof(s_Data.EditorCamera->GetPosition()), &s_Data.EditorCamera->GetPosition());
					shader->UploadUniform("u_PointLightCount", sizeof(s_Data.PointLightCount), &s_Data.PointLightCount);
				}
			}

			s_Data.NumberOfDrawCalls = 0;
			s_Data.QuadIndexCount = 0;
			s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
			s_Data.TextureSlotIndex = 1;
		}
		void Renderer2D::BeginScene(Camera* camera, const glm::mat4& transform)
		{
			s_Data.RuntimeCamera = camera;

			glm::mat4 viewProjMatrix = s_Data.RuntimeCamera->GetProjectionMatrix() * glm::inverse(transform);
			ShaderLibrary::GetShader("2Shader")->UploadUniform("u_ViewProjMat", sizeof(viewProjMatrix), &viewProjMatrix);

			glm::mat4 viewMatrix = transform;
			viewMatrix = glm::mat4(glm::mat3(viewMatrix));
			auto viewProj = camera->GetProjectionMatrix() * viewMatrix;
			ShaderLibrary::GetShader("SkyboxShader")->UploadUniform("u_ViewProjMat", sizeof(viewProj), &viewProj);

			s_Data.NumberOfDrawCalls = 0;
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
		void Renderer2D::ClearColor(const glm::vec4& color)
		{
			glClearColor(color.r, color.g, color.b, color.a);
		}
		void Renderer2D::Clear()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		void Renderer2D::SetOrthographicCamera(const Ref<OrthographicCamera> camera)
		{
			s_Data.OrthographicCamera = camera;
		}
		void Renderer2D::SetEditorCamera(const Ref<EditorCamera> camera)
		{
			s_Data.EditorCamera = camera;
		}
		uint32_t Renderer2D::GetIndexCount()
		{
			return s_Data.QuadIndexCount;
		}
		uint32_t Renderer2D::GetNumberOfDrawCalls()
		{
			return s_Data.NumberOfDrawCalls;
		}
	}
}
