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
			static void RenderDirectionalLightShadowMap(Ref<Scene> scene);
			static void RenderPointLightShadowMaps(Ref<Scene> scene);
			static void DrawCube(const TransformComponent& tc, const MeshRendererComponent& mc, const TagComponent& tagc);
			static void DrawSkybox();
			static void Shutdown() {};
			//Will become deprecated soon.
			static void SetOrthographicCamera(const Ref<OrthographicCamera> camera);
			//---
			static void Submit(const TransformComponent& tc, SpriteRendererComponent& sc, int entityID);
			static void Flush();
			static void SetEditorCamera(const Ref<EditorCamera> camera);
			static void BeginScene(const Ref<EditorCamera>& camera);
			static void EndScene();
			static void Clear();
			static void ClearColor(const glm::vec4& color);
			static uint32_t GetIndexCount();
			static uint32_t GetNumberOfDrawCalls();
		};
	}
}