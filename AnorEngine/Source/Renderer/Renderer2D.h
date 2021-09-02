#pragma once
#include "Graphics/Buffers/VertexArray.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/Camera.h"
#include "Graphics/EditorCamera.h"
namespace AnorEngine
{
	//Forward declerations.
	class TransformComponent;
	class SpriteRendererComponent;
	class MeshRendererComponent;
	class TagComponent;
	class PointLightComponent;
	class Scene;
	struct FramebufferData
	{
		unsigned int fboID;
		unsigned int depthTextureID;
	};
	namespace Graphics
	{
		class ANOR_API Renderer2D
		{
			struct Statistics
			{
				static uint32_t QuadIndicesCount;
			};
		public:
			static void Init();
			static void ClearColor(const glm::vec4& color);
			static void Clear();
			static void Shutdown() {};
			static void SetEditorCamera(const Ref<EditorCamera> camera);
			static void RenderDirectionalLightShadowMap(Ref<Scene> scene);
			static void RenderPointLightShadowMap(Ref<Scene> scene);
			//Will become deprecated soon.
			static void SetOrthographicCamera(const Ref<OrthographicCamera> camera);
			//---
			static void SetPointLightCount(int count);
			static void SetPointLightInAllShaders(const TransformComponent& tc, const MeshRendererComponent& mc, const PointLightComponent& plc, int index);
			static void DrawCube(const TransformComponent& tc, const MeshRendererComponent& mc, const TagComponent& tagc);
			static void DrawSkybox();
			//static void Submit(const glm::vec3& position, const glm::vec2& size, const Ref<Texture> texture, float rotationDegree = 0.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f } );
			//static void Submit(const glm::vec3& position, const glm::vec2& size, const Ref<Texture> texture, const glm::vec2& subTextureOffset, const glm::vec2& subTextureDimensions, float rotationDegree = 0.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
			//static void Submit(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
			//static void Submit(const glm::mat4& transform, glm::vec4& color);
			//static void Submit(const glm::mat4& transform, const Ref<Texture> texture, float rotationDegree = 0.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
			//static void Submit(const glm::mat4& transform, const glm::vec2& size, const Ref<Texture> texture, const glm::vec2& subTextureOffset, const glm::vec2& subTextureDimensions, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
			//This submit is here for convenience only. Above ones require too many parameters and sometimes it is a pain in the neck to debug and stuff.
			static void Submit(const TransformComponent& tc, SpriteRendererComponent& sc, int entityID);
			static void Flush();
			static void BeginScene(const Ref<EditorCamera>& camera);
			static void BeginScene(Camera* camera, const glm::mat4& transform);
			static void EndScene();
		public:
			static uint32_t GetIndexCount();
			static uint32_t GetNumberOfDrawCalls();
		private:
			static void GenerateDirectionalLightFramebuffer();
			static FramebufferData GeneratePointLightFramebuffer();
		};
	}
}