#pragma once
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <imgui.h>
namespace AnorEngine
{
	class ANOR_API SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);
		void SetContext(const Ref<Scene>& scene);
		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
		template <typename T>
		void DrawComponent(const char* componentName, Entity entity, std::function<void(void)> fnc)
		{
			if (entity.HasComponent<T>())
			{
				if (ImGui::TreeNodeEx((void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, componentName))
				{
					fnc();
					ImGui::TreePop();
				}
			}
		}
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}