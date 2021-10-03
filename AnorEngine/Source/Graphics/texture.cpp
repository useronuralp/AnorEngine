#include "pch.h"
#include <glad/glad.h>
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

extern const std::filesystem::path g_AssetPath = (std::string(__SOLUTION_DIR) + "AnorEngine\\Assets");
extern const std::filesystem::path g_ResourcesPath = (std::string(__SOLUTION_DIR) + "AnorEditor");
namespace AnorEngine {
	//!!!! DEFAULT GLFW FRAMEBUFFER TEXTURE ID IS 0 !!!!!
	//! 
	//!!!! WHITE TEXTURE ID IS 1 !!!!!!
	//!!!! SKYBOX TEXTURE ID IS 2 !!!!!
	//!!!! A SINGLE FRAMEBUFFER WILL ALLOCATE 3 TEXTURE IDs (2 FOR COLOR, 1 FOR DEPTH) !!!!!
	//!!!! THEN WILL COME THE TEXTURES YOU LOAD !!!!
	namespace Graphics {
		Texture::Texture(const std::filesystem::path& relativePath, const char* fileType)
		{		
			m_RelativeFilePath = relativePath.string();
			if (fileType == "Asset")
				m_AbsoluteFilePath = g_AssetPath.string() + "\\" + relativePath.string();
			else if(fileType == "Resource")
				m_AbsoluteFilePath = g_ResourcesPath.string() + "\\" + relativePath.string();
			SetupTexture();
		}
		void Texture::SetupTexture()
		{
			int width, height, channels;
			stbi_uc* data = stbi_load(m_AbsoluteFilePath.c_str(), &width, &height, &channels, 0);
			m_Width = width;
			m_Channels = channels;
			m_Height = height;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}
			//Texture IDs start from number 1. Number 0 means that the texture is either invalid or it was deleted.
			glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
			glTextureStorage2D(m_TextureID, 1, internalFormat, m_Width, m_Height);

			glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//minify scaling
			glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//magnify scaling
			glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
			if (data)
			{
				stbi_image_free(data);
			}
			else
			{
				CRITICAL_ASSERT("Textures failed to load from path: {0}", m_AbsoluteFilePath);
				stbi_image_free(data);
			}
		}
		Texture::~Texture()
		{
			glDeleteTextures(1, &m_TextureID);
		}
		void Texture::Bind(int slot) const
		{
			glActiveTexture(GL_TEXTURE0 + (slot % 32)); //allows you to speicfy a texture slot, usually on pc there are 32 texture.
			glBindTexture(GL_TEXTURE_2D, m_TextureID);
		}
		void Texture::Unbind() const
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		CubeMapTexture::CubeMapTexture(std::vector<std::string> faces)
		{
			unsigned int textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
			int width, height, nrChannels;
			for (unsigned int i = 0; i < faces.size(); i++)
			{
				//flipping images doesnt work!!
				stbi_set_flip_vertically_on_load(true);
				stbi_uc* data = stbi_load((g_AssetPath.string() + "\\" + faces[i]).c_str(), &width, &height, &nrChannels, 0);
				if (data)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
					);
					m_Faces[i].m_Channels = nrChannels;
					m_Faces[i].m_AbsoluteFilePath = g_AssetPath.string() + "\\" + faces[i];
					m_Faces[i].m_Height = height;
					m_Faces[i].m_Width = width;		
					stbi_image_free(data);
				}
				else
				{
					stbi_image_free(data);
					CRITICAL_ASSERT("Cubemap tex failed to load at path: {0}", faces[i]);
				}
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			m_TextureID = textureID;
		}
		void CubeMapTexture::Bind() const
		{
			glActiveTexture(GL_TEXTURE0 + (m_TextureID % 32));
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
		}
		void CubeMapTexture::Unbind() const
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
	}
}