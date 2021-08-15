#pragma once
namespace AnorEngine {
	namespace Graphics {

		class ANOR_API Texture
		{
		private:
			
			std::string m_FilePath;
			uint32_t m_RendererID, m_Width, m_Height, m_Channels;
		public:
			Texture(const std::string& path);
			~Texture();

			void Bind(unsigned int slot) const;
			void Unbind() const;
			inline int GetWidth() const { return m_Width; }
			inline int GetHeight() const { return m_Height; }
			inline unsigned int GetTextureID() const{ return m_RendererID; }


			inline std::string GetPath() { return m_FilePath; }
			bool operator == (const Texture& other) const
			{
				return m_RendererID == other.m_RendererID;
			}
		};
		struct CubeMapTextureFace
		{
			std::string m_FilePath;
			uint32_t m_Width;
			uint32_t m_Height;
			uint32_t m_Channels;
		};
		class ANOR_API CubeMapTexture
		{
		private:
			uint32_t m_TextureID;
			CubeMapTextureFace m_Faces[6];
		public:
			CubeMapTexture(std::vector<std::string> faces);
			uint32_t GetTextureID() { return m_TextureID; }
			void Bind(unsigned int slot) const;
			void Unbind() const;
		};
	}
}