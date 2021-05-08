#pragma once
#include <GL/glew.h>
#include "../utils/fileutils.h"
#include "../Math/maths.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
namespace AnorEngine {
	namespace Graphics {

		class ANOR_API Shader {

		private:
			GLuint m_ShaderID;
			const char* m_VertPath;
			const char* m_FragPath;
		public:
			Shader(const char* vertPath, const char* fragPath);
			~Shader();		
			void setUniform1f(const GLchar* name, const float value);
			void setUniform1fv(const GLchar* name, float* value,int count);
			void setUniform1i(const GLchar* name, const int value);
			void setUniform1iv(const GLchar* name, int* value, int count);
			void setUniform2f(const GLchar* name, const Math::vec2 &vector);
			void setUniform2f(const std::string name, const Math::vec2& vector);
			void setUniform3f(const GLchar* name, const Math::vec3 &vector);
			void setUniform3f(const std::string name, const Math::vec3& vector);
			void setUniform4f(const GLchar* name, const Math::vec4 &vector);
			void setUniformMat4(const GLchar* name, const Math::mat4& matrix);
			void UploadMat4(const char* name, const glm::mat4& matrix);
			void enable() const;
			void disable() const;
		private:
			GLuint load();
			GLint getUniformLocation(const GLchar* name);
			GLint getUniformLocation(std::string name);
		public:
			inline GLuint getShaderID() const { return m_ShaderID; }
		};
	}
}