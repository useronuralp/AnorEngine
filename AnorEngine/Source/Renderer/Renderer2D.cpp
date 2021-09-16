#include "pch.h"
#include "Renderer2D.h"
#include "Scene/Components.h"
#include "Graphics/Framebuffer.h"

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
		struct PointLightProperties
		{
			PointLightProperties(const TransformComponent& t, const MeshRendererComponent& m, const PointLightComponent& p, int nmbr)
				:tc(t), mc(m), plc(p), number(nmbr){}

			TransformComponent tc;
			MeshRendererComponent mc;
			PointLightComponent plc;
			int number;
		};
		struct Renderer2DData //Used to store data in this static class. Defined this struct in the .cpp so that nothing else can include it.
		{
			static constexpr int					  MAX_POINT_LIGHT_COUNT = 10;
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

			std::array<Ref<Texture>, MaxTextureSlots> StoredTextures;
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

			//Directional light
			Ref<Framebuffer>					      DirectionalLightShadowBuffer;
			glm::vec3								  DirectionalLightPosition = { 120.0f, 200.0f, -200.0f };
			glm::vec3								  DirectionalLightColor = { 1.0f, 1.0f, 1.0f };

			//Point lights
			Ref<Framebuffer>						  PointLightShadowBuffers[MAX_POINT_LIGHT_COUNT];
			std::vector<PointLightProperties>         PointLights;
			int										  PointLightCount = 0;
		};

		//Instance the s_Data here for only once.
		static Renderer2DData s_Data;
		static Ref<Framebuffer> GenerateDirectionalLightFramebuffer()
		{
			FramebufferSpecifications specs;
			specs.Height = 7680;
			specs.Width = 7680;
			specs.Attachments = { {FramebufferTextureFormat::Depth} };
			specs.Texture_Type = TextureType::TEXTURE_2D;

			return std::make_shared<Framebuffer>(specs);
		}
		static Ref<Framebuffer> GeneratePointLightFramebuffer()
		{
			FramebufferSpecifications specs;
			specs.Height = 1024;
			specs.Width = 1024;
			specs.Attachments = { {FramebufferTextureFormat::Depth} };
			specs.Texture_Type = TextureType::CUBEMAP;

			return std::make_shared<Framebuffer>(specs);
		}
		void Renderer2D::Init()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_DEPTH_TEST);

			s_Data.DirectionalLightShadowBuffer = nullptr;
			for (int i = 0; i < s_Data.MAX_POINT_LIGHT_COUNT; i++)
				s_Data.PointLightShadowBuffers[i] = nullptr;


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
				{ShaderDataType::vec2, "a_UV", 2},
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
			ShaderLibrary::LoadShader("SkyboxShader", "Shaders\\CubemapShader.shader");
			ShaderLibrary::LoadShader("CubeShader", "Shaders\\CubeShader.shader");
			ShaderLibrary::LoadShader("LightCubeShader", "Shaders\\LightCubeShader.shader");
			ShaderLibrary::LoadShader("2DShader", "Shaders\\2DShader.shader");
			ShaderLibrary::LoadShader("ShadowShader", "Shaders\\Shadow.shader");
			ShaderLibrary::LoadShader("PointLightShadowShader", "Shaders\\PointLightShadow.shader");
			ShaderLibrary::LoadShader("3DShader", "Shaders\\3DShader.shader");

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
			s_Data.WhiteTexture = std::make_shared<Texture>("Textures\\WhiteTexture.png"); //TODO: Create this texture without a using a PNG.
			s_Data.StoredTextures[0] = s_Data.WhiteTexture;

			std::vector<std::string> cubeMapFaces
			{
				"Textures\\Skyboxes\\skybox\\left.jpg",
				"Textures\\Skyboxes\\skybox\\right.jpg",
				"Textures\\Skyboxes\\skybox\\top.jpg",
				"Textures\\Skyboxes\\skybox\\bottom.jpg",
				"Textures\\Skyboxes\\skybox\\front.jpg",
				"Textures\\Skyboxes\\skybox\\back.jpg"
			};

			
			s_Data.SkyboxTexture = std::make_shared<CubeMapTexture>(cubeMapFaces);


			//Explicitly defining the vertex positions of the quads here so that we can do transform operations on these in CPU side.
			s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		}
		void Renderer2D::RenderScene(const Ref<Scene>& scene, const Ref<Shader>& shader, bool wannaCheckforPointLights)
		{
			//----- Models
			auto viewModel = scene->GetRegistry().view<TransformComponent, ModelRendererComponent, MeshRendererComponent, TagComponent>();
			for (auto& entity : viewModel)
			{
				auto [transformComponent, modelComponent, meshRendererComponent, tagComponent] = viewModel.get<TransformComponent, ModelRendererComponent, MeshRendererComponent, TagComponent>(entity);
				auto transform = transformComponent.GetTransform();
				shader->UploadUniform("u_Transform", sizeof(transform), &transform);
				Graphics::Renderer2D::DrawModel(transformComponent, modelComponent, meshRendererComponent, tagComponent, shader);
				shader->Disable();
			}
			//----- Models

 
			//----- Cubes
			auto viewCube = scene->GetRegistry().view<TransformComponent, MeshRendererComponent, TagComponent>();
			for (auto& entity : viewCube)
			{
				auto [transformComponent, meshRendererComponent, tagComponent] = viewCube.get<TransformComponent, MeshRendererComponent, TagComponent>(entity);
				auto transform = transformComponent.GetTransform();

				shader->UploadUniform("u_Transform", sizeof(transform), &transform);
				s_Data.CubeVertexArray->Bind();

				if (tagComponent.Tag != "Model")
				{
					if (wannaCheckforPointLights)
					{
						if (tagComponent.Tag != "Point Light")
							glDrawArrays(GL_TRIANGLES, 0, 36);
					}
					else
						glDrawArrays(GL_TRIANGLES, 0, 36);
				}
				s_Data.CubeVertexArray->Unbind();
				shader->Disable();
			}
			//----- Cubes

		}
		void Renderer2D::RenderPointLightShadowMaps(const Ref<Scene>& scene)
		{
			//----- Loop through the point lights in the scene
			s_Data.PointLights.clear();
			auto pointLightView = scene->GetRegistry().view<TransformComponent, MeshRendererComponent, TagComponent, PointLightComponent>();
			int PointLightCount = 0;
			for (auto& entity : pointLightView)
			{
				auto [transformComponent, meshRendererComponent, tagComponent, pointLightComponent] = pointLightView.get<TransformComponent, MeshRendererComponent, TagComponent, PointLightComponent>(entity);
				for (auto& [shaderName, shader] : ShaderLibrary::GetLibrary())
				{
					shader->UploadUniform("u_PointLights[" + std::to_string(PointLightCount) + "].color", sizeof(meshRendererComponent.Color), &meshRendererComponent.Color);
					shader->UploadUniform("u_PointLights[" + std::to_string(PointLightCount) + "].position", sizeof(transformComponent.Translation), &transformComponent.Translation);
					shader->UploadUniform("u_PointLights[" + std::to_string(PointLightCount) + "].constant", sizeof(float), &pointLightComponent.Constant);
					shader->UploadUniform("u_PointLights[" + std::to_string(PointLightCount) + "].Linear", sizeof(pointLightComponent.Linear), &pointLightComponent.Linear);
					shader->UploadUniform("u_PointLights[" + std::to_string(PointLightCount) + "].quadratic", sizeof(pointLightComponent.Quadratic), &pointLightComponent.Quadratic);
					shader->UploadUniform("u_PointLights[" + std::to_string(PointLightCount) + "].intensity", sizeof(pointLightComponent.Intensity), &pointLightComponent.Intensity);
					shader->UploadUniform("u_PointLights[" + std::to_string(PointLightCount) + "].ambient", sizeof(meshRendererComponent.Material->Properties.Ambient), &meshRendererComponent.Material->Properties.Ambient);
					shader->UploadUniform("u_PointLights[" + std::to_string(PointLightCount) + "].diffuse", sizeof(meshRendererComponent.Material->Properties.Diffuse), &meshRendererComponent.Material->Properties.Diffuse); 
					shader->UploadUniform("u_PointLights[" + std::to_string(PointLightCount) + "].specular", sizeof(meshRendererComponent.Material->Properties.Specular), &meshRendererComponent.Material->Properties.Specular);
					shader->UploadUniform("u_PointLights[" + std::to_string(PointLightCount) + "].castShadow", sizeof(pointLightComponent.CastPointLightShadow), &pointLightComponent.CastPointLightShadow);
				}
				PointLightProperties props(transformComponent, meshRendererComponent, pointLightComponent, PointLightCount);
				//store all the point lights in the scene.
				s_Data.PointLights.push_back(props);
				if (PointLightCount < s_Data.MAX_POINT_LIGHT_COUNT)
					PointLightCount++;
			}
			s_Data.PointLightCount = PointLightCount;
			//----- Loop through the point lights in the scene
			
			//If theres no point lights in the scene no need to continue.
			if (PointLightCount == 0)
				return;

			//Rendering
			for (int i = 0; i < PointLightCount; i++)
			{
				if (!s_Data.PointLightShadowBuffers[i])
					s_Data.PointLightShadowBuffers[i] = GeneratePointLightFramebuffer();


				float near_plane = 0.1f;
				float far_plane = 50.0f;
				glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)1024 / (float)1024, near_plane, far_plane);
				glm::mat4 shadowTransforms[6];

				//Direction vectors to sample from a cubemap
				shadowTransforms[0] = shadowProj * glm::lookAt(s_Data.PointLights[i].tc.Translation, s_Data.PointLights[i].tc.Translation + glm::vec3( 1.0,  0.0,  0.0),  glm::vec3(0.0, -1.0,  0.0));
				shadowTransforms[1] = shadowProj * glm::lookAt(s_Data.PointLights[i].tc.Translation, s_Data.PointLights[i].tc.Translation + glm::vec3(-1.0,  0.0,  0.0),  glm::vec3(0.0, -1.0,  0.0));
				shadowTransforms[2] = shadowProj * glm::lookAt(s_Data.PointLights[i].tc.Translation, s_Data.PointLights[i].tc.Translation + glm::vec3( 0.0,  1.0,  0.0),  glm::vec3(0.0,  0.0,  1.0));
				shadowTransforms[3] = shadowProj * glm::lookAt(s_Data.PointLights[i].tc.Translation, s_Data.PointLights[i].tc.Translation + glm::vec3( 0.0, -1.0,  0.0),  glm::vec3(0.0,  0.0, -1.0));
				shadowTransforms[4] = shadowProj * glm::lookAt(s_Data.PointLights[i].tc.Translation, s_Data.PointLights[i].tc.Translation + glm::vec3( 0.0,  0.0,  1.0),  glm::vec3(0.0, -1.0,  0.0));
				shadowTransforms[5] = shadowProj * glm::lookAt(s_Data.PointLights[i].tc.Translation, s_Data.PointLights[i].tc.Translation + glm::vec3( 0.0,  0.0, -1.0),  glm::vec3(0.0, -1.0,  0.0));

				ShaderLibrary::GetShader("PointLightShadowShader")->UploadUniform("u_ShadowMatrices[0]", sizeof(shadowTransforms) , &shadowTransforms);
				ShaderLibrary::GetShader("PointLightShadowShader")->UploadUniform("u_Far_plane", sizeof(far_plane), &far_plane);
				ShaderLibrary::GetShader("PointLightShadowShader")->UploadUniform("u_LightPos", sizeof(s_Data.PointLights[i].tc.Translation), &s_Data.PointLights[i].tc.Translation);
				for (auto& [shaderName, shader] : ShaderLibrary::GetLibrary())
					shader->UploadInteger("u_PointLightShadowMap[" + std::to_string(i) + "]", s_Data.PointLightShadowBuffers[i]->GetDepthAttachmentID());

				for (auto& [shaderName, shader] : ShaderLibrary::GetLibrary())
				{
					shader->UploadUniform("u_PointLigthFarPlane", sizeof(far_plane), &far_plane);
				}

				s_Data.PointLightShadowBuffers[i]->Bind();
				glClear(GL_DEPTH_BUFFER_BIT);
				RenderScene(scene, ShaderLibrary::GetShader("PointLightShadowShader"), true);
				s_Data.PointLightShadowBuffers[i]->Unbind();

			}
		}
		void Renderer2D::RenderDirectionalLightShadowMap(const Ref<Scene>& scene)
		{
			if (!s_Data.DirectionalLightShadowBuffer)
				s_Data.DirectionalLightShadowBuffer = GenerateDirectionalLightFramebuffer();

			float near_plane = 0.1f, far_plane = 1000.0f;
			glm::mat4 directionalLightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);
			glm::mat4 directionalLightView = glm::lookAt(s_Data.DirectionalLightPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 directionalightViewProjMatrix = directionalLightProjection * directionalLightView;
			for (auto& [shaderName, shader] : ShaderLibrary::GetLibrary())
			{
				//Directional light should be single and fixed. And thus, I am storing the properties of it here in the renderer.
				shader->UploadUniform("u_DirectionalShadowMap", sizeof(s_Data.DirectionalLightShadowBuffer->GetDepthAttachmentID()), &s_Data.DirectionalLightShadowBuffer->GetDepthAttachmentID());
				shader->UploadUniform("u_DirectionalLightPosition", sizeof(s_Data.DirectionalLightPosition), &s_Data.DirectionalLightPosition);
				shader->UploadUniform("u_DirectionalLightColor", sizeof(s_Data.DirectionalLightColor), &s_Data.DirectionalLightColor);
				//Upload directional light view projection matrix.
				shader->UploadUniform("u_DirectionalLightViewProjMatrix", sizeof(directionalightViewProjMatrix), &directionalightViewProjMatrix);
				shader->UploadUniform("u_DirectionalLightNear_plane", sizeof(near_plane), &near_plane);
				shader->UploadUniform("u_DirectionalLightFar_plane", sizeof(far_plane), &far_plane);
			}

			s_Data.DirectionalLightShadowBuffer->Bind();
			glClear(GL_DEPTH_BUFFER_BIT);
			RenderScene(scene, ShaderLibrary::GetShader("ShadowShader"));
			s_Data.DirectionalLightShadowBuffer->Unbind();
		}
		void Renderer2D::DrawCube(const TransformComponent& tc, const MeshRendererComponent& mc, const TagComponent& tagc)
		{
			//Component specific shader uniforms are set inside this funciton.
			mc.Material->Shader->UploadUniform("u_TextureSamplerDiffuse", sizeof(mc.Material->Texture->GetTextureID()), &mc.Material->Texture->GetTextureID());
			mc.Material->Shader->UploadUniform("u_CastDirectionalLight", sizeof(mc.CastDirectionalLight), &mc.CastDirectionalLight);
			mc.Material->Shader->UploadUniform("u_Transform", sizeof(tc.GetTransform()), &tc.GetTransform());

			mc.Material->Shader->UploadUniform("u_Color", sizeof(mc.Color), &mc.Color);
			mc.Material->Shader->UploadUniform("u_EntityID", sizeof(tc.EntityID), &tc.EntityID);

			mc.Material->Shader->UploadUniform("u_Material.diffuseIntensity", sizeof(mc.Material->Properties.Diffuse), &mc.Material->Properties.Diffuse);
			mc.Material->Shader->UploadUniform("u_Material.ambientIntensity", sizeof(mc.Material->Properties.Ambient), &mc.Material->Properties.Ambient);
			mc.Material->Shader->UploadUniform("u_Material.specularIntensity", sizeof(mc.Material->Properties.Specular), &mc.Material->Properties.Specular);
			mc.Material->Shader->UploadUniform("u_Material.shininess", sizeof(mc.Material->Properties.Shininess), &mc.Material->Properties.Shininess);
			mc.Material->Shader->UploadUniform("u_Material.metalness", sizeof(mc.Material->Properties.Metalness), &mc.Material->Properties.Metalness);

			mc.Material->Texture->Bind(mc.Material->Texture->GetTextureID());
			mc.Material->Shader->Enable();
			s_Data.CubeVertexArray->Bind();

			glDrawArrays(GL_TRIANGLES, 0, 36);

			s_Data.CubeVertexArray->Unbind();
			mc.Material->Shader->Disable();
			mc.Material->Texture->Unbind();
		}
		void Renderer2D::DrawModel(const TransformComponent& tc, const ModelRendererComponent& model, const MeshRendererComponent& mc, const TagComponent& tagc, const Ref<Shader>& differentShader)
		{
			mc.Material->Shader->UploadUniform("u_Transform", sizeof(tc.GetTransform()), &tc.GetTransform());
			mc.Material->Shader->UploadUniform("u_EntityID", sizeof(tc.EntityID), &tc.EntityID);
			mc.Material->Shader->UploadUniform("u_Color", sizeof(mc.Color), &mc.Color);
			mc.Material->Shader->UploadUniform("u_CastDirectionalLight", sizeof(mc.CastDirectionalLight), &mc.CastDirectionalLight);

			mc.Material->Shader->UploadUniform("u_Material.diffuseIntensity", sizeof(mc.Material->Properties.Diffuse), &mc.Material->Properties.Diffuse);
			mc.Material->Shader->UploadUniform("u_Material.ambientIntensity", sizeof(mc.Material->Properties.Ambient), &mc.Material->Properties.Ambient);
			mc.Material->Shader->UploadUniform("u_Material.specularIntensity", sizeof(mc.Material->Properties.Specular), &mc.Material->Properties.Specular);
			mc.Material->Shader->UploadUniform("u_Material.shininess", sizeof(mc.Material->Properties.Shininess), &mc.Material->Properties.Shininess);
			mc.Material->Shader->UploadUniform("u_Material.metalness", sizeof(mc.Material->Properties.Metalness), &mc.Material->Properties.Metalness);

			if (differentShader) // this "differentShader" ususally refers to a shadow map shader. And can be passed to this function by either "RenderDirectionalShadowMap()" or "RenderPointLightShadowMaps()" functions.
				model.Model->Draw(differentShader);
			else // If you're not using an external shader, use the shader that is stored inside the material of the object.
				model.Model->Draw(mc.Material->Shader);
		}
		void Renderer2D::DrawSkybox()
		{
			glDepthMask(GL_FALSE);
			glBindTexture(GL_TEXTURE_CUBE_MAP, s_Data.SkyboxTexture->GetTextureID());
			auto viewProjMatrix = s_Data.EditorCamera->GetProjectionMatrix() * glm::mat4(glm::mat3(s_Data.EditorCamera->GetViewMatrix()));
			ShaderLibrary::GetShader("SkyboxShader")->UploadUniform("u_ViewProjMat", sizeof(viewProjMatrix), &(viewProjMatrix));
			s_Data.CubeVertexArray->Bind();

			glDrawArrays(GL_TRIANGLES, 0, 36);
			glDepthMask(GL_TRUE);

			s_Data.CubeVertexArray->Unbind();
			ShaderLibrary::GetShader("SkyboxShader")->Disable();
			//THIS IS GONNA CAUSE PROBLEMS CLEAR THE CORRECT FRAMEBUFFER ATTACHMENT
			//TODO: Hard coded the color attachment index as 4 because it is known at the moment.
			int clearValue = -1;
			glClearTexImage(4, 0, GL_RED_INTEGER, GL_INT, &clearValue);
		}
		Ref<Texture> Renderer2D::CreateTexture(const std::filesystem::path& relativePath)
		{
			for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			{
				if (s_Data.StoredTextures[i])
				{
					//TODO : you should probably check for absolute / relative paths here.
					if (s_Data.StoredTextures[i]->GetPath() == relativePath.string())
					{
						return s_Data.StoredTextures[i];
					}
				}
			}
			Ref<Texture> newTexture = std::make_shared<Texture>(relativePath);
			for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			{
				if (!s_Data.StoredTextures[i])
				{
					//Caching the newly created txture here.
					s_Data.StoredTextures[i] = newTexture;
					break;
				}
			}
			return newTexture;
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
				if (*s_Data.StoredTextures[i].get() == *sc.Texture.get())
				{
					textureIndex = (float)i;
					break;
				}
			}
			if (textureIndex == 0.0f)
			{
				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.StoredTextures[s_Data.TextureSlotIndex] = sc.Texture;
				s_Data.TextureSlotIndex++;
			}

			//This scaling is neccessary to match the sizes of the textures and required quads to draw them.
			glm::mat4 transformEdited = glm::scale(tc.GetTransform(), { sc.TextureSize.x, sc.TextureSize.y, 1.0f });

			s_Data.QuadVertexBufferPtr->Position = transformEdited * s_Data.QuadVertexPositions[0];
			s_Data.QuadVertexBufferPtr->Color = sc.Color;
			s_Data.QuadVertexBufferPtr->TexCoord = { sc.SubTextureOffset.x * (sc.PixelSizeOfEachCell.x / sc.Texture->GetWidth()), sc.SubTextureOffset.y * (sc.PixelSizeOfEachCell.y / sc.Texture->GetHeight()) };
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = transformEdited * s_Data.QuadVertexPositions[1];
			s_Data.QuadVertexBufferPtr->Color = sc.Color;
			s_Data.QuadVertexBufferPtr->TexCoord = { (sc.SubTextureOffset.x + sc.TextureSize.x) * (sc.PixelSizeOfEachCell.x / sc.Texture->GetWidth()), sc.SubTextureOffset.y * (sc.PixelSizeOfEachCell.y / sc.Texture->GetHeight()) };
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = transformEdited * s_Data.QuadVertexPositions[2];
			s_Data.QuadVertexBufferPtr->Color = sc.Color;
			s_Data.QuadVertexBufferPtr->TexCoord = { (sc.SubTextureOffset.x + sc.TextureSize.x) * (sc.PixelSizeOfEachCell.x / sc.Texture->GetWidth()), (sc.SubTextureOffset.y + sc.TextureSize.y) * (sc.PixelSizeOfEachCell.y / sc.Texture->GetHeight()) };
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = transformEdited * s_Data.QuadVertexPositions[3];
			s_Data.QuadVertexBufferPtr->Color = sc.Color;
			s_Data.QuadVertexBufferPtr->TexCoord = { sc.SubTextureOffset.x * (sc.PixelSizeOfEachCell.x / sc.Texture->GetWidth()), (sc.SubTextureOffset.y + sc.TextureSize.y) * (sc.PixelSizeOfEachCell.y / sc.Texture->GetHeight()) };
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
				s_Data.StoredTextures[i]->Bind(i);
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
			s_Data.EditorCamera = camera;
			//Common uniforms for every shader are set here.
			for (auto& [shaderName, shader] : ShaderLibrary::GetLibrary())
			{	
				shader->UploadUniform("u_ViewProjMat", sizeof(s_Data.EditorCamera->GetViewProjectionMatrix()), &s_Data.EditorCamera->GetViewProjectionMatrix());
				shader->UploadUniform("u_CameraPos", sizeof(s_Data.EditorCamera->GetPosition()), &s_Data.EditorCamera->GetPosition());		
				shader->UploadUniform("u_PointLightCount", sizeof(s_Data.PointLightCount), &s_Data.PointLightCount);
			}

			s_Data.NumberOfDrawCalls = 0;
			s_Data.QuadIndexCount = 0;
			s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
			s_Data.TextureSlotIndex = 1;
			DrawSkybox();


			//Bind the shadow maps.
			s_Data.DirectionalLightShadowBuffer->BindDepthAttachmentTexture();
			for (int i = 0; i < s_Data.PointLightCount; i++)
			{
				s_Data.PointLightShadowBuffers[i]->BindDepthAttachmentTexture();
			}
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
		glm::vec3& Renderer2D::GetDirectionalLightPosition()
		{
			return s_Data.DirectionalLightPosition;
		}
		const std::array<Ref<Texture>, 32Ui64>& Renderer2D::GetStoredTextures()
		{
			return s_Data.StoredTextures;
		}
	}
}
