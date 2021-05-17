#pragma once
#include "../vendor/stb_image/stb_image.h"
#include <GL/glew.h>

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

			void Bind(unsigned int slot = 0) const;
			void Unbind() const;

			inline int GetWidth() const { return m_Width; }
			inline int GetHeight() const { return m_Height; }
			inline unsigned int getTID() const{ return m_RendererID; }
		};
	}
}