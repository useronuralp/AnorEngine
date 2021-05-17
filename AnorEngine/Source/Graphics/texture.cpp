#include "pch.h"
#include "texture.h"

namespace AnorEngine {
	namespace Graphics {
		Texture::Texture(const std::string& path)
			:m_FilePath(path)
		{
			int width, height, channels;
			stbi_set_flip_vertically_on_load(1); //in OpenGL the texture (0,0) starts at bottom left. Regular images you load need to be flipped in order to alignh with that rule.
			stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
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


			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);


			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR );//minify scaling
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//magnify scaling

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
			if (data)
				stbi_image_free(data);
		}

		Texture::~Texture()
		{
			glDeleteTextures(1, &m_RendererID);
		}

		void Texture::Bind(unsigned int slot) const
		{
			//glActiveTexture(GL_TEXTURE0 + slot); //allows you to speicfy a texture slot, usually on pc there are 32 texture slots and theu count up one by one.
			glBindTexture(GL_TEXTURE_2D, m_RendererID);
		}

		void Texture::Unbind() const
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}