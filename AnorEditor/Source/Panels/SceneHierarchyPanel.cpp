#include "pch.h"
#include "SceneHierarchyPanel.h"
#include <Scene/Components.h>
#include <imgui.h>
namespace AnorEngine
{
	
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}
	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
	}
	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			//Grabbing the tag component of an entity by reference here.
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			//Create a local buffer and set its content to 0 so it is clear.
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			//ImGui statements can be checked in an if statement cause they return booleans.
			//ImGui::InputText returns true if the buffer you send in to that function was modified.
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				//If the buffer was modified we change the tag component of the entity with the newly typed in tag.
				tag = std::string(buffer);
			}
		}
		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{		
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				//You can modify the transform translation values (x,y) with an ImGui DragFloat3 like this.
				if (ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.01f))
				{
					;
				}
				ImGui::TreePop();
			}

		}
		if (entity.HasComponent<CameraComponent>())
		{
			auto& camera = entity.GetComponent<CameraComponent>().Camera;

			if (camera.GetProjectionType() == ProjectionType::Perspective)
			{
				float FOV = camera.GetPerspectiveFOV();
				if (ImGui::DragFloat("FOV", &FOV))
					camera.SetPerspectiveFOV(FOV);
				float perspectiveNear = camera.GetNearClipPerspective();
				if (ImGui::DragFloat("Near", &perspectiveNear))
					camera.SetNearClipPerspective(perspectiveNear);
				float perspectiveFar = camera.GetFarClipPerspective();
				if (ImGui::DragFloat("Far", &perspectiveFar))
					camera.SetFarClipPerspective(perspectiveFar);
			}
			else
			{
				float orthoSize = camera.GetOrhographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					camera.SetOrthoGraphicSize(orthoSize);
				float orthoNear = camera.GetNearClipOrthographic();
				if (ImGui::DragFloat("Near", &orthoNear))
					camera.SetNearClipOrthographic(orthoNear);
				float orthoFar = camera.GetFarClipOrthographic();
				if (ImGui::DragFloat("Far", &orthoFar))
					camera.SetFarClipOrthographic(orthoFar);
			}
		}
		if (entity.HasComponent<SpriteRendererComponent>())
		{
			auto& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();
			ImGui::Separator();
			ImGui::ColorEdit4("Color", glm::value_ptr(spriteRenderer.Color));
			ImGui::DragFloat2("Repeat Textures",  glm::value_ptr(spriteRenderer.TextureSize));
			ImGui::Separator();
		}
	}
	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		m_Context->m_Registry.each([&](auto entityID)
		{
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNode(entity);
		});

		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};
		ImGui::End();

		ImGui::Begin("Properties");
		//Check to see if there is something assigned to selection context, if so, draw its components.
		if (m_SelectionContext)
			DrawComponents(m_SelectionContext);
		ImGui::End();
	}
	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ?  ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}
		if (opened)
		{
			ImGui::TreePop();
		}
	}
}