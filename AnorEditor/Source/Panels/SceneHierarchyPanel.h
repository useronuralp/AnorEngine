#pragma once
#include "Scene/Entity.h"
namespace AnorEngine
{
	//Forward declerations
	class Scene;

	class ANOR_API SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);
		void SetSelectionContext(int entityID) { m_SelectionContext = Entity{ (entt::entity)entityID, m_Context.get() }; }
		void SetContext(const Ref<Scene>& scene);
		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
		template <typename Component, typename UIFunction>
		void DrawComponent(const char* componentName, Entity entity, UIFunction uiFunction)
		{
			if (entity.HasComponent<Component>())
			{
				//Grabbing T component of an entity by reference here.
				auto& component = entity.GetComponent<Component>();
				float availableContentRegion = ImGui::GetContentRegionAvail().x;
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
				ImGui::Separator();
				bool open = ImGui::TreeNodeEx((void*)typeid(Component).hash_code(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap, componentName);

				ImGui::PopStyleVar();
				ImGui::SameLine(availableContentRegion - lineHeight * 0.5f);

				if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight }))
				{
					ImGui::OpenPopup("Component Settings");
				}
				ImGui::Separator();
				bool removeComponent = false;
				if (ImGui::BeginPopup("Component Settings"))
				{
					if (ImGui::MenuItem("Remove Component"))
						removeComponent = true;
					ImGui::EndPopup();
				}
				if (open)
				{
					uiFunction(component);
					ImGui::TreePop();
				}
				if (removeComponent)
					entity.RemoveComponent<Component>();
			}
		}
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}