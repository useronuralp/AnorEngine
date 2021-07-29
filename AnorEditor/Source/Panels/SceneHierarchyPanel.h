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
				bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap, componentName);
				ImGui::SameLine();
				if (ImGui::Button("+"))
				{
					ImGui::OpenPopup("Component Settings");

				}
				bool removeComponent = false;
				if (ImGui::BeginPopup("Component Settings"))
				{
					if (ImGui::MenuItem("Remove Component"))
						removeComponent = true;
					ImGui::EndPopup();
				}
				if (open)
				{
					fnc();
					ImGui::TreePop();
				}
				if (removeComponent)
					entity.RemoveComponent<T>();
			}
		}
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}