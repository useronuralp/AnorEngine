#include "pch.h"
#include "SceneHierarchyPanel.h"
#include <Scene/Components.h>
#include <imgui.h>
#include <imgui_internal.h>
namespace AnorEngine
{
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float dragSensetivity = 0.01f, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y + 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, dragSensetivity, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.9f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, dragSensetivity, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, dragSensetivity, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}
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
		DrawComponent<TagComponent>("Tag", entity, [&entity]()
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
		});

		DrawComponent<TransformComponent>("Transform", entity, [&entity]()
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			//You can modify the transform translation values (x,y) with an ImGui DragFloat3 like this.
			DrawVec3Control("Position", transform.Translation);
			glm::vec3 rotationDegree = glm::degrees(transform.Rotation);
			DrawVec3Control("Rotation", rotationDegree , 1.0f);
			transform.Rotation = glm::radians(rotationDegree);
			DrawVec3Control("Scale", transform.Scale, 0.1f, 1.0f);
		});

		DrawComponent<CameraComponent>("Camera", entity, [&entity]()
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
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [&entity]()
		{
			if (entity.HasComponent<SpriteRendererComponent>())
			{
				auto& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();
				ImGui::Separator();
				ImGui::ColorEdit4("Color", glm::value_ptr(spriteRenderer.Color));
				ImGui::DragFloat2("Repeat Textures", glm::value_ptr(spriteRenderer.TextureSize));
				ImGui::Separator();
			}
		});

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