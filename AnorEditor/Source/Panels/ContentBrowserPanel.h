#pragma once
namespace AnorEngine
{	
	//Forward declerations
	namespace Graphics
	{
		class Texture;
	}
	class ANOR_API ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDirectory;
		Ref<Graphics::Texture> m_DirectoryIcon;
		Ref<Graphics::Texture> m_FileIcon;
		Ref<Graphics::Texture> m_BackIcon;
	};
}