#include "pch.h"
#include "ContentBrowserPanel.h"

#include <imgui.h>

namespace AnorEngine
{
	static const std::filesystem::path s_AssetPath = (std::string(__SOLUTION_DIR) + "AnorEngine\\Assets");

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(s_AssetPath){}
	
	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != std::filesystem::path(s_AssetPath))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		for(auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_AssetPath);
			std::string relativePathString = relativePath.string();
			if (directoryEntry.is_directory())
			{
				if (ImGui::Button(relativePathString.c_str()))
				{
					m_CurrentDirectory /= path.filename();
				}
			}
			else
			{
				if (ImGui::Button(relativePathString.c_str()))
				{
				}
			}
		}
		ImGui::End();
	}
}

