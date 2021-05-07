#pragma once
#include "../vendor/stb_image/stb_image.h"
#include <GL/glew.h>

namespace AnorEngine {
	namespace Graphics {

		class ANOR_API Texture
		{
		private:
			
			GLuint m_RendererID;
			GLubyte* m_LocaBuffer;
			std::string m_FilePath;
			int m_Width, m_Height, m_BPP; 

		public:
			Texture(const std::string& path);
			~Texture();

			void Bind(unsigned int slot = 0) const;
			void Unbind() const;

			inline int GetWidth() const { return m_Width; }
			inline int GetHeight() const { return m_Height; }
			inline GLuint getTID() const{ return m_RendererID; }
		};
	}
}