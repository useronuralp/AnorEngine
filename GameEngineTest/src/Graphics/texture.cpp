#include "pch.h"
#include "texture.h"

namespace GameEngineTest {
	namespace Graphics {
		Texture::Texture(const std::string& path)
			:m_FilePath(path)
		{
			stbi_set_flip_vertically_on_load(1); //in OpenGL the texture (0,0) starts at bottom left. Regular images you load need to be flipped in order to alignh with that rule.
			m_LocaBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//minify scaling
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//magnify scaling
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //if you dont speicfy these 4, you'll get a blank texture.
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocaBuffer);
			glBindTexture(GL_TEXTURE_2D, 0);

			if (m_LocaBuffer)
				stbi_image_free(m_LocaBuffer);
		}

		Texture::~Texture()
		{
			glDeleteTextures(1, &m_RendererID);
		}

		void Texture::Bind(unsigned int slot) const
		{
			glActiveTexture(GL_TEXTURE0 + slot); //allows you to speicfy a texture slot, usually on pc there are 32 texture slots and theu count up one by one.
			glBindTexture(GL_TEXTURE_2D, m_RendererID);
		}

		void Texture::Unbind() const
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}