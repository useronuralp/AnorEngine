#pragma once
#include <glm.hpp>
namespace AnorEngine
{
	//Forward declerations.
	class TransformComponent;
	class SpriteRendererComponent;
	class MeshRendererComponent;
	class TagComponent;
	class PointLightComponent;
	class ModelRendererComponent;
	class Scene;


	enum class TextureNames
	{
		WHITE_TEXTURE = 0
	};

	namespace Graphics
	{
		
		//Forward declerations.
		class Framebuffer;
		class Layer;
		class Texture;
		class m_Shader;
		class OrthographicCamera;
		class EditorCamera;
		class Camera;
		class Shader;


		class ANOR_API Renderer2D
		{
			struct Statistics
			{
				static uint32_t QuadIndicesCount;
			};
		public:
			static void Init();
			static void RenderScene(Ref<Layer> layer, bool isRuntime, float deltaTime);
			static void PreRender();
			static void PostRender();
			static void RenderDirectionalLightShadowMap(const Ref<Scene>& scene);
			static void RenderPointLightShadowMaps(const Ref<Scene>& scene);
			static void DrawCube(const TransformComponent& tc, const MeshRendererComponent& mc, const TagComponent& tagc);
			static void DrawModel(const TransformComponent& tc, const ModelRendererComponent& model, const MeshRendererComponent& mc, const TagComponent& tagc, const Ref<Shader>& differentShader = nullptr);
			static void DrawSkybox();
			static void Shutdown() {};
			//Will become deprecated soon.
			static void SetOrthographicCamera(const Ref<OrthographicCamera> camera);
			//---
			static Ref<Texture> CreateTexture(const std::filesystem::path& relativePath);
			static void Submit(const TransformComponent& tc, SpriteRendererComponent& sc, int entityID);
			static void Flush();
			static void SetEditorCamera(const Ref<EditorCamera> camera);
			static void BeginScene(const Ref<EditorCamera>& camera);
			static void EndScene();
			static void Clear();
			static void ClearColor(const glm::vec4& color);
			static uint32_t									GetIndexCount();
			static uint32_t									GetNumberOfDrawCalls();
			static glm::vec3&								GetDirectionalLightPosition();
			static const std::array<Ref<Texture>, 32Ui64>&	GetStoredTextures();
			static Ref<Framebuffer>							GetMainFramebuffer();
		private:
			static void RenderScene(const Ref<Scene>& scene, const Ref<Shader>& shader, bool wannaCheckforPointLights = false);
		};
	}
}