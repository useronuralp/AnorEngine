#include "pch.h"
#include "ContentBrowserPanel.h"

#include "Core\AnorColors.h"
#include <imgui.h>
namespace AnorEngine
{
	static const std::filesystem::path s_AssetPath = (std::string(__SOLUTION_DIR) + "AnorEngine\\Assets");

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(s_AssetPath)
	{
		m_DirectoryIcon = std::make_shared<Graphics::Texture>(std::string(__SOLUTION_DIR) + "AnorEditor\\Resources\\Icons\\DirectoryIcon.jpg");
		m_FileIcon = std::make_shared<Graphics::Texture>(std::string(__SOLUTION_DIR) + "AnorEditor\\Resources\\Icons\\FileIcon.png");
		m_BackIcon = std::make_shared<Graphics::Texture>(std::string(__SOLUTION_DIR) + "AnorEditor\\Resources\\Icons\\BackIcon.png");
	}
	
	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");
		auto& colors = ImGui::GetStyle().Colors;
		
		if (m_CurrentDirectory != std::filesystem::path(s_AssetPath))
		{
			if (ImGui::ImageButton((void*)m_BackIcon->GetTextureID(), { 20.0f,20.0f }, {0,1}, {1,0}))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}


		static float padding = 16.0f;
		static float thumbnailSisze = 80.0f;
		float cellSize = thumbnailSisze + padding;



		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0 , false);
		int i = 0;
		for(auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			ImGui::PushID(i++);
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_AssetPath);
			std::string filenameString = relativePath.string();

			auto textureID = directoryEntry.is_directory() ? m_DirectoryIcon->GetTextureID() : m_FileIcon->GetTextureID();

			
			ImGui::PushStyleColor(ImGuiCol_Button, AnorColors::DefaultButtonColor);

			ImGui::ImageButton((void*)textureID, { thumbnailSisze, thumbnailSisze }, { 0, 1 }, { 1, 0 }, -1, {0,0,0,0});
			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
				ImGui::ImageButton((void*)textureID, { 50, 50 } , { 0, 1 }, { 1, 0 });
				ImGui::EndDragDropSource();
			}
			if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
				{
					m_CurrentDirectory /= path.filename();
				}
			}
			ImGui::PopStyleColor();
			ImGui::PopID();
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
		}


		ImGui::Columns(1);

		//ImGui::SliderFloat("Thumbnails Size", &thumbnailSisze, 16, 512);
		//ImGui::SliderFloat("Padding Size", &padding, 0, 32);
		ImGui::End();
	}
}

