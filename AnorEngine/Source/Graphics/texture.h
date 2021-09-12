#pragma once
namespace AnorEngine {
	namespace Graphics {

		class ANOR_API Texture
		{
		private:
			std::string m_RelativeFilePath = "Not initialized";
			std::string m_AbsoluteFilePath;
			uint32_t m_TextureID;
			uint32_t m_Width;
			uint32_t m_Height;
			uint32_t m_Channels;
			std::string m_Type = "Not initialized";
		private:
			void SetupTexture();
		public:
			Texture(const std::filesystem::path& relativePath, const char* fileType = "Asset");
			~Texture();

			void CreateTextureWithRelativePath(std::filesystem::path relativePath);
			void Bind(unsigned int slot) const;
			void Unbind() const;
			inline int GetWidth() const { return m_Width; }
			inline int GetHeight() const { return m_Height; }
			inline const unsigned int& GetTextureID() const { return m_TextureID; }

			inline void SetType(const std::string& type) { m_Type = type; }
			inline std::string GetType() { return m_Type; }

			inline std::string GetPath() { return m_RelativeFilePath == "Not initialized" ? m_AbsoluteFilePath : m_RelativeFilePath; }
			bool operator == (const Texture& other) const
			{
				return m_TextureID == other.m_TextureID;
			}
		};
		struct CubeMapTextureFace
		{
			std::string m_AbsoluteFilePath;
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