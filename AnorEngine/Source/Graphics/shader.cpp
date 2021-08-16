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

		Shader::Shader(std::string name, std::string vertPath, std::string fragPath)
			:m_Name(name)
		{
			std::unordered_map<GLenum, std::string> shaderSources;
			shaderSources[GL_VERTEX_SHADER] = ReadFile(vertPath);
			shaderSources[GL_FRAGMENT_SHADER] = ReadFile(fragPath);
			m_VertPath = vertPath;
			m_FragPath = fragPath;
			Compile(shaderSources);
		}
		Shader::Shader(std::string name, std::string filepath)
			:m_Name(name)
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

				GLint result = 0;
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

			GLint isLinked = 0;
			glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
			if (isLinked == GL_FALSE)
			{
				CRITICAL_ASSERT("Failed to link shaders!, {0}");
				GLint maxLength = 0;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

				for (auto& shaderID : shaderIDs)
				{
					glDeleteShader(shaderID);
				}
				glDeleteProgram(program);
				
				return;
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
			if (in)
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
		void Shader::UploadFloat(const std::string name, const float value)
		{
			enable();
			glUniform1f(GetUniformLocation(name), value);
		}
		void Shader::UploadFloatArray(const std::string name, float* value, int count)
		{
			enable();
			glUniform1fv(GetUniformLocation(name), count, value);
		}
		void Shader::UploadInteger(const std::string name, const int value)
		{
			enable();
			glUniform1i(GetUniformLocation(name), value);
		}
		void Shader::UploadIntegerArray(const GLchar* name, int* value, int count)
		{
			enable();
			glUniform1iv(GetUniformLocation(name), count, value);
		}
		void Shader::UploadFloat2(const std::string name, const glm::vec2& vector)
		{
			enable();
			glUniform2f(GetUniformLocation(name), vector.x, vector.y);
		}
		void Shader::UploadFloat3(const std::string name, const glm::vec3& vector)
		{
			enable();
			glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z);
		}
		void Shader::UploadFloat4(const std::string name, const glm::vec4& vector)
		{
			enable();
			glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
		}
		void Shader::UploadMat4(const std::string name, const glm::mat4& matrix)
		{
			enable();
			glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
		}

		void Shader::UploadIntegerArray(const std::string name, int* values, uint32_t count)
		{
			enable();
			glUniform1iv(GetUniformLocation(name), count, values);
		}

		std::unordered_map<std::string, Ref<Shader>> ShaderLibrary::m_Shaders;
		Ref<Shader> ShaderLibrary::AddShader(const Ref<Shader>& shader)
		{
			std::string name = shader->GetName();
			//if the shader already exists in the library. We return a reference to that.
			for (auto& shdr : m_Shaders)
			{
				if (shdr.first == name)
				{
					WARN("The shader you wanted to load is already present in the shader library.");		
					return m_Shaders[name];
				}
			}
			//if the shader we want to add to the library does not exist in the library, we create a new shader, add it to the library and then retun a reference to the newly created shader.
			return m_Shaders[name] = shader;
		}
		Ref<Shader> ShaderLibrary::LoadShader(const std::string& name, const std::string& filepath)
		{
			return AddShader(std::make_shared<Shader>(name, filepath));
		}
	}
}