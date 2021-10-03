#pragma once
#include <glm.hpp>
#include "Utility/fileutils.h"
namespace AnorEngine {
	namespace Graphics {

		//Forward declerations:
		using GLenum = unsigned int;
		using GLint = int;
		using GLuint = unsigned int;

		struct UniformData
		{
			std::string Name;
			GLenum      Type; //float, vec3, mat4, etc
			GLint       Count; // if the uniform is inside an array for example.
		};

		class ANOR_API Shader {
		private:
			GLuint										 m_ShaderID;
			std::string									 m_Name;
			std::string									 m_VertPath = "Empty";
			std::string									 m_FragPath = "Empty";
			std::string									 m_AbsoluteFilePath = "Empty";
			std::unordered_map<std::string, GLint>       m_UniformLocationCache;
			std::unordered_map<std::string, UniformData> m_UniformBuffer;
		public:
			Shader(const std::string& name, const std::string& vertPath, const std::string& fragPath);
			Shader(const std::string& name, const std::string& relativeFilePath);
			~Shader();	
		public:
			inline const std::string& GetName() const { return m_Name; }
		public:
			void UploadUniform(const std::string& uniformName, size_t size, const void* data);
			void Enable();
			void Disable() const;
		public:
			void UploadFloat(const std::string& name, const float value);
			void UploadFloatArray(const std::string& name, float* value,int count);
			void UploadInteger(const std::string& name, const int value);
			void UploadFloat2(const std::string& name, const glm::vec2& vector);
			void UploadFloat3(const std::string& name, const glm::vec3& vector);
			void UploadFloat4(const std::string& name, const glm::vec4& vector);
			void UploadMat4(const std::string& name, const glm::mat4& matrix);
			void UploadMat4Array(const std::string& name,const glm::mat4& matrix, const int& count);
			void UploadIntegerArray(const std::string& name, int* values, uint32_t count);
		public:
			void FillUniformBuffer();
			std::string ReadFile(const std::string& filepath);
			std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
			void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
			GLint GetUniformLocation(const std::string& name);
		public:
			inline const std::string GetFilePath() const { return m_AbsoluteFilePath; }
			inline GLuint getShaderID() const { return m_ShaderID; }
		};

		//This library will hold a reference to every shader you create in your program. Which means every shader will have a minimum of 1 strong reference in your program.
		class ANOR_API ShaderLibrary
		{
		public:
			static std::unordered_map<std::string, Ref<Shader>>& GetLibrary() { return m_Shaders; }
			//Returns a shader reference.
			static Ref<Shader> LoadShader(const std::string& name, const std::string& filepath);
			//Returns a shader reference.
			static Ref<Shader> AddShader(const Ref<Shader>& shader);
			//Returns an empty pointer if the specified shader was not found.
			static Ref<Shader> GetShader(const std::string& shaderName)
			{
				if (m_Shaders.find(shaderName) != m_Shaders.end())
					return m_Shaders[shaderName];
				else
				{
					CRITICAL_ASSERT("The Shader you tried to load is invalid. (Either the name is incorrect or there is no shader with that name in the Shader Library.)");
				}
			}
		public:
			static std::unordered_map<std::string, Ref<Shader>>::iterator begin() { return m_Shaders.begin(); }
			static std::unordered_map<std::string, Ref<Shader>>::iterator end() { return m_Shaders.end(); }
		private:
			static std::unordered_map<std::string, Ref<Shader>> m_Shaders;
		};
	}
}