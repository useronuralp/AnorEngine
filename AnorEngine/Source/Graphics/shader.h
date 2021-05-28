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
			std::string m_Name;
			std::string m_VertPath = "Empty";
			std::string m_FragPath = "Empty";
			std::string m_FilePath = "Empty";
		public:
			Shader(std::string name, std::string vertPath, std::string fragPath);
			Shader(std::string name, std::string filepath);
			~Shader();	
		public:
			inline const std::string& GetName() const { return m_Name; }
		public:
			void UploadFloat(const GLchar* name, const float value);
			void UploadFloatArray(const GLchar* name, float* value,int count);
			void UploadInteger(const GLchar* name, const int value);
			void UploadIntegerArray(const GLchar* name, int* value, int count);
			void UploadFloat2(const GLchar* name, const Math::vec2 &vector);
			void UploadFloat2(const std::string name, const Math::vec2& vector);
			void UploadFloat3(const GLchar* name, const Math::vec3 &vector);
			void UploadFloat3(const std::string name, const Math::vec3& vector);
			void UploadFloat4(const GLchar* name, const glm::vec4& vector);
			void UploadMat4(const char* name, const glm::mat4& matrix);
			void enable() const;
			void disable() const;
		private:
			std::string ReadFile(std::string filepath);
			std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
			void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
			GLint GetUniformLocation(const GLchar* name);
			GLint GetUniformLocation(std::string name);
		public:
			inline const std::string GetFilePath() const { return m_FilePath; }
			inline GLuint getShaderID() const { return m_ShaderID; }
		};

		//This library will hold a reference to every shader you create in your program. Which means every shader will have a minimum of 1 strong reference in your program.
		class ANOR_API ShaderLibrary
		{
		public:
			//Returns a shader reference.
			static Ref<Shader> LoadShader(const std::string& name, const std::string& filepath);
			//Returns a shader reference.
			static Ref<Shader> AddShader(const Ref<Shader>& shader);
			//Returns an empty pointer if the specified shader was not found.
			static Ref<Shader> GetShader(const std::string& shaderName)
			{
				if (m_Shaders.count(shaderName) > 0)
					return m_Shaders[shaderName];
				else
					CRITICAL_ASSERT("The Shader you tried to load is invalid. (Either the name is incorrect or there is no shader with that name in the Shader Library.)");
			}
		private:
			static std::unordered_map<std::string, Ref<Shader>> m_Shaders;
		};
	}
}