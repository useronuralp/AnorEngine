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
		auto& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, dragSensetivity, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.9f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, dragSensetivity, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, dragSensetivity, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		io.FontDefault = io.Fonts->Fonts[1];
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		std::string solutionDir = __SOLUTION_DIR;
		m_PlayButtonTexture = std::make_shared<Graphics::Texture>(solutionDir + "AnorEngine\\Assets\\Textures\\play.png");
		m_PauseButtonTexture = std::make_shared<Graphics::Texture>(solutionDir + "AnorEngine\\Assets\\Textures\\pause.png");
		SetContext(scene);
	}
	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
		m_SelectionContext = {};
	}
	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		//Grabbing Tag component of an entity by reference here.
		auto& component = entity.GetComponent<TagComponent>();
		float availableContentRegion = ImGui::GetContentRegionAvail().x;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::PopStyleVar();

		ImGui::SameLine(availableContentRegion - lineHeight * 0.5f);

		ImGui::Separator();
		//Create a local buffer and set its content to 0 so it is clear.
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), component.Tag.c_str());
		//ImGui statements can be checked in an if statement cause they return booleans.
		//ImGui::InputText returns true if the buffer you send in to that function was modified.
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			//If the buffer was modified we change the tag component of the entity with the newly typed in tag.
			component.Tag = std::string(buffer);
		}


		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("Add Component");

		if (ImGui::BeginPopup("Add Component"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				auto& cameraComponent = m_SelectionContext.AddComponent<CameraComponent>();
				cameraComponent.Camera.SetViewportSize(m_Context->m_ViewportWidth, m_Context->m_ViewportHeight);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Sprite Renderer"))
			{
				m_SelectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();
		ImGui::Separator();
		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			DrawVec3Control("Position", component.Translation);
			glm::vec3 rotationDegree = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotationDegree , 1.0f);
			component.Rotation = glm::radians(rotationDegree);
			DrawVec3Control("Scale", component.Scale, 0.1f, 1.0f);
		});
		DrawComponent<MeshRendererComponent>("Mesh Renderer", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			ImGui::DragFloat("Ambient", &component.Material.Properties.Ambient, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Diffuse", &component.Material.Properties.Diffuse, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Specular", &component.Material.Properties.Specular, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Shininess", &component.Material.Properties.Shininess);
			ImGui::DragFloat("Metalness", &component.Material.Properties.Metalness, 0.01f, 0.0f, 1.0f);


		});
		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			if (component.Camera.GetProjectionType() == ProjectionType::Perspective)
			{
				float FOV = component.Camera.GetPerspectiveFOV();
				if (ImGui::DragFloat("FOV", &FOV))
					component.Camera.SetPerspectiveFOV(FOV);
				float perspectiveNear = component.Camera.GetNearClipPerspective();
				if (ImGui::DragFloat("Near", &perspectiveNear))
					component.Camera.SetNearClipPerspective(perspectiveNear);
				float perspectiveFar = component.Camera.GetFarClipPerspective();
				if (ImGui::DragFloat("Far", &perspectiveFar))
					component.Camera.SetFarClipPerspective(perspectiveFar);
				ImGui::Checkbox("Is Primary", &component.Primary);
				auto& projectionType = component.Camera.GetProjectionType();
				if (ImGui::TreeNodeEx("Camera Projection"))
				{
					if (ImGui::SmallButton("Orhtographic"))
					{
						projectionType = ProjectionType::Orthographic;
						component.Camera.RecalculateProjectionMatrix();
					}
					if (ImGui::SmallButton("Perspective"))
					{
						projectionType = ProjectionType::Perspective;
						component.Camera.RecalculateProjectionMatrix();
					}
					ImGui::TreePop();
				}
			}
			else
			{

				float orthoSize = component.Camera.GetOrhographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					component.Camera.SetOrthoGraphicSize(orthoSize);
				float orthoNear = component.Camera.GetNearClipOrthographic();
				if (ImGui::DragFloat("Near", &orthoNear))
					component.Camera.SetNearClipOrthographic(orthoNear);
				float orthoFar = component.Camera.GetFarClipOrthographic();
				if (ImGui::DragFloat("Far", &orthoFar))
					component.Camera.SetFarClipOrthographic(orthoFar);
				ImGui::Checkbox("Is Primary", &component.Primary);

				auto& projectionType = component.Camera.GetProjectionType();
				if(ImGui::TreeNodeEx("Camera Projection"))
				{
					if (ImGui::SmallButton("Orhtographic"))
					{
						projectionType = ProjectionType::Orthographic;
					}
					if (ImGui::SmallButton("Perspective"))
					{
						projectionType = ProjectionType::Perspective;
					}
					ImGui::TreePop();
				}
			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
		{
				ImGui::Separator();
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::DragFloat2("Repeat Textures", glm::value_ptr(component.TextureSize));
				ImGui::Separator();
		});
		DrawComponent<NativeScriptComponent>("Script", entity, [](auto& component)
		{
			ImGui::Separator();
			ImGui::Checkbox("Is Enabled", &component.isEnabled);
			ImGui::Separator();
		});

	}
	bool SceneHierarchyPanel::OnImGuiRender()
	{
		int texID = m_IsRuntime ? m_PauseButtonTexture->GetTextureID() : m_PlayButtonTexture->GetTextureID();
		ImGui::Begin("Scene Hierarchy");

		if (ImGui::ImageButton((void*)texID, ImVec2{ 50.0f, 50.0f }))
			m_IsRuntime = !m_IsRuntime;

		m_Context->m_Registry.each([&](auto entityID)
		{
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNode(entity);
		});

		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		if (ImGui::BeginPopupContextWindow(0 ,1, false))
		{
			if (ImGui::MenuItem("Create Entity"))
				m_Context->CreateEntity();
			if (ImGui::MenuItem("Cube"))
			{
				Graphics::Material defaultMaterial(Graphics::ShaderLibrary::GetShader("CubeShader"));
				m_Context->CreateEntity().AddComponent<MeshRendererComponent>(defaultMaterial);
			}
			if (ImGui::MenuItem("Cube Light"))
			{
				Graphics::Material defaultMaterial (Graphics::ShaderLibrary::GetShader("LightCubeShader"));
				auto entity = m_Context->CreateEntity();
				entity.AddComponent<MeshRendererComponent>(defaultMaterial);
				entity.GetComponent<TagComponent>().Tag = "Point Light";
			}
			ImGui::EndPopup();
		}
		ImGui::End();

		ImGui::Begin("Properties");
		//Check to see if there is something assigned to selection context, if so, draw its components.
		if (m_SelectionContext)
			DrawComponents(m_SelectionContext);

		
		ImGui::End();
		return m_IsRuntime;
	}
	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ?  ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;
			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)6846456, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted)
		{		
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext = entity)
				m_SelectionContext = {};
		}
	}
}