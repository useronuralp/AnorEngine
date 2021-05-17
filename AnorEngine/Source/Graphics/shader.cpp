#include "pch.h"
#include "shader.h"
namespace AnorEngine {
	namespace Graphics {	

		static GLenum ShaderTypeFromString(std::string type)
		{
			if (type == "vertex") 
				return GL_VERTEX_SHADER;
			if (type == "pixel" || type == "fragment")
				return GL_FRAGMENT_SHADER;
		}

		Shader::Shader(std::string vertPath, std::string fragPath)
		{	
			std::unordered_map<GLenum, std::string> shaderSources;
			shaderSources[GL_VERTEX_SHADER] = ReadFile(vertPath);
			shaderSources[GL_FRAGMENT_SHADER] = ReadFile(fragPath);
			m_VertPath = vertPath;
			m_FragPath = fragPath;
			Compile(shaderSources);
		}
		Shader::Shader(std::string filepath)
		{	
			m_FilePath = filepath;
			std::string source = ReadFile(filepath);
			std::unordered_map<GLenum, std::string> shaderSources = PreProcess(source);
			Compile(shaderSources);
		}
		void Shader::enable() const
		{
			glUseProgram(m_ShaderID);
		}
		void Shader::disable() const
		{
			glUseProgram(0);
		}
		Shader::~Shader()
		{
			glDeleteProgram(m_ShaderID);
		}

		void Shader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
		{
			GLuint program = glCreateProgram();
			std::vector<GLuint> shaderIDs;
			for (const auto& item : shaderSources)
			{
				GLenum type = item.first;
				const char* source = item.second.c_str();

				GLuint shader = glCreateShader(type);
				glShaderSource(shader, 1, &source, NULL);
				glCompileShader(shader);

				GLint result;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

				if (!result) {
					GLint length;
					glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
					std::vector<char> error(length);
					glGetShaderInfoLog(shader, length, &length, &error[0]);
					glDeleteShader(shader);
					std::string typeString;
					if (type == 35633)
						typeString = "vertex";
					else if (type == 35632)
						typeString = "fragment";
					CRITICAL_ASSERT("Failed to compile {0} shader, {1}", typeString, &error[0]);
				}
				glAttachShader(program, shader);
				shaderIDs.push_back(shader);
			}

			glLinkProgram(program);
			glValidateProgram(program);

			GLint result;
			glGetProgramiv(program, GL_LINK_STATUS, &result);
			if (!result) {
				GLint length;
				glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);
				std::vector<char> error(length);
				glGetShaderInfoLog(program, length, &length, &error[0]);
				for (auto& shaderID : shaderIDs)
				{
					glDeleteShader(shaderID);
				}
				CRITICAL_ASSERT("Failed to link shaders!, {0}", &error[0]);
				glDeleteShader(program);
			}

			for (auto& shaderID : shaderIDs)
			{
				glDetachShader(program, shaderID);
			}

			m_ShaderID = program;
		}
		std::string Shader::ReadFile(std::string filepath)
		{
			std::string result;
			std::ifstream in(filepath, std::ios::in | std::ios::binary);
			if(in)
			{
				in.seekg(0, std::ios::end);
				result.resize(in.tellg());
				in.seekg(0, std::ios::beg);
				in.read(&result[0], result.size());
				in.close();
			}
			else
			{
				CRITICAL_ASSERT("Specified shader file is invalid. (Did you make a typo while entering the path?)");
			}
			return result;
		}
		std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source)
		{
			std::unordered_map<GLenum, std::string> shaderSources;

			const char* typeToken = "#type";
			size_t typeTokenLength = strlen(typeToken);
			size_t pos = source.find(typeToken, 0);
			while (pos != std::string::npos)
			{
				size_t eol = source.find_first_of("\r\n", pos);
				if (eol == std::string::npos)
				{
					CRITICAL_ASSERT("Type identifier syntax error in shader.");
				}
				size_t begin = pos + typeTokenLength + 1;
				std::string type = source.substr(begin, eol - begin);
				if (type == "vertex" || type == "fragment" || type == "pixel")
				{
				}
				else
				{
					CRITICAL_ASSERT("Invalid shader type or Shader syntax error");
				}

				size_t nextLinePos = source.find_first_not_of("\n\r", eol);
				pos = source.find(typeToken, nextLinePos);
				shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
			}
			return shaderSources;
		}

		GLint Shader::GetUniformLocation(const GLchar* name)
		{
			return glGetUniformLocation(m_ShaderID, name);
		}
		GLint Shader::GetUniformLocation(std::string name)
		{
			char* arr;
			arr = &name[0];
			return glGetUniformLocation(m_ShaderID, arr);
		}
		void Shader::UploadFloat(const GLchar* name, const float value)
		{	
			enable();
			glUniform1f(GetUniformLocation(name), value);
		}
		void Shader::UploadFloatArray(const GLchar* name, float* value ,int count)
		{	
			enable();
			glUniform1fv(GetUniformLocation(name), count, value);
		}
		void Shader::UploadInteger(const GLchar* name, const int value) 
		{	
			enable();
			glUniform1i(GetUniformLocation(name), value);
		}
		void Shader::UploadIntegerArray(const GLchar* name, int* value, int count)
		{	
			enable();
			glUniform1iv(GetUniformLocation(name), count, value);
		}
		void Shader::UploadFloat2(const GLchar* name, const Math::vec2& vector) 
		{
			enable();
			glUniform2f(GetUniformLocation(name), vector.x, vector.y);
		}
		void Shader::UploadFloat2(const std::string name , const Math::vec2& vector)
		{
			enable();
			glUniform2f(GetUniformLocation(name), vector.x, vector.y);
		}
		void Shader::UploadFloat3(const GLchar* name, const Math::vec3& vector)
		{
			enable();
			glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z);
		}
		void Shader::UploadFloat3(const std::string name, const Math::vec3& vector)
		{
			enable();
			glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z);
		}
		void Shader::UploadFloat4(const GLchar* name, const glm::vec4& vector)
		{
			enable();
			glUniform4f(GetUniformLocation(name), vector.x ,vector.y, vector.z, vector.w);
		}
		void Shader::UploadMat4(const char* name, const glm::mat4& matrix)
		{
			enable();
			glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
		}
	}
}