#pragma once

#include <filesystem>

namespace AnorEngine
{
	class ANOR_API ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDirectory;
	};
}