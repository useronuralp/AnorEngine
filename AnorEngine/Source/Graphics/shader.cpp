#include "pch.h"
#include "shader.h"
namespace AnorEngine {
	//Globals, defined in texture.h
	extern const std::filesystem::path g_AssetPath;
	extern const std::filesystem::path g_ResourcesPath;

	namespace Graphics {

		// TODO: !!!WARNING!!! DATA SIZES MAY BE INCORRECT DOUBLE CHECK THEM LATER.
		static size_t GetSizeInBytesFromGLDataType(GLenum type)
		{
			switch (type)
			{
				case GL_BOOL:			   return 1;	     break;
				case GL_SAMPLER:
				case GL_SAMPLER_2D:
				case GL_SAMPLER_CUBE:
				case GL_INT:
				case GL_UNSIGNED_BYTE:
				case GL_UNSIGNED_INT:
				case GL_FLOAT:			   return 4;		 break;
				case GL_DOUBLE:			   return 8;         break;
				case GL_BOOL_VEC2:		   return 2;		 break;
				case GL_BOOL_VEC3:		   return 3;		 break;
				case GL_BOOL_VEC4:		   return 4;		 break;
				case GL_INT_VEC2:		   return 2 * 4;	 break;
				case GL_INT_VEC3:		   return 3 * 4;	 break;
				case GL_INT_VEC4:		   return 4 * 4;	 break;
				case GL_UNSIGNED_INT_VEC2: return 2 * 4;	 break;
				case GL_UNSIGNED_INT_VEC3: return 3 * 4;	 break;
				case GL_UNSIGNED_INT_VEC4: return 4 * 4;	 break;
				case GL_FLOAT_VEC2:		   return 2 * 4;	 break;
				case GL_FLOAT_VEC3:		   return 3 * 4;	 break;
				case GL_FLOAT_VEC4:		   return 4 * 4;	 break;
				case GL_DOUBLE_VEC2:	   return 2 * 8;	 break;
				case GL_DOUBLE_VEC3:	   return 3 * 8;	 break;
				case GL_DOUBLE_VEC4:	   return 4 * 8;	 break;
				case GL_FLOAT_MAT2:		   return 2 * 2 * 4; break;
				case GL_FLOAT_MAT2x3:	   return 2 * 3 * 4; break;
				case GL_FLOAT_MAT2x4:	   return 2 * 4 * 4; break;
				case GL_FLOAT_MAT3:		   return 3 * 3 * 4; break;
				case GL_FLOAT_MAT3x2:      return 3 * 2 * 4; break;
				case GL_FLOAT_MAT3x4:      return 3 * 4 * 4; break;
				case GL_FLOAT_MAT4:		   return 4 * 4 * 4; break;
				case GL_FLOAT_MAT4x3:	   return 4 * 3 * 4; break;
				case GL_FLOAT_MAT4x2:	   return 4 * 2 * 4; break;
				case GL_DOUBLE_MAT2:	   return 2 * 2 * 8; break;
				case GL_DOUBLE_MAT2x3:	   return 2 * 3 * 8; break;
				case GL_DOUBLE_MAT2x4:	   return 2 * 4 * 8; break;
				case GL_DOUBLE_MAT3:	   return 3 * 3 * 8; break;
				case GL_DOUBLE_MAT3x2:	   return 3 * 2 * 8; break;
				case GL_DOUBLE_MAT3x4:	   return 3 * 4 * 8; break;
				case GL_DOUBLE_MAT4:	   return 4 * 4 * 8; break;
				case GL_DOUBLE_MAT4x3:	   return 4 * 3 * 8; break;
				case GL_DOUBLE_MAT4x2:	   return 4 * 2 * 8; break;
			}
		}
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
		Shader::Shader(std::string name, std::string relativeFilePath)
			:m_Name(name)
		{
			m_AbsoluteFilePath = g_AssetPath.string() + relativeFilePath;
			std::string source = ReadFile(m_AbsoluteFilePath);
			std::unordered_map<GLenum, std::string> shaderSources = PreProcess(source);
			Compile(shaderSources);

			//Reads the shader and retrieves all of the active uniforms.
			FillUniformBuffer();		
		}
		void Shader::FillUniformBuffer()
		{
			GLint i;
			GLint count;

			GLint size; // size of the variable
			GLenum type; // type of the variable (float, vec3 or mat4, etc)

			const GLsizei bufferSize = 64; // maximum name length
			GLchar uniformName[bufferSize]; // variable name in GLSL
			GLsizei length; // name length


			glGetProgramiv(m_ShaderID, GL_ACTIVE_UNIFORMS, &count);
			printf("Shader Name: %s, Active Attributes: %d\n", m_Name.c_str(), count);
			for (i = 0; i < count; i++)
			{
				glGetActiveUniform(m_ShaderID, (GLuint)i, bufferSize, &length, &size, &type, uniformName);
				m_UniformBuffer.insert(std::make_pair<std::string, UniformData>(uniformName, UniformData{ uniformName, type, size }));
				//m_UniformBuffer.push_back(UniformData{ uniformName, type, size, malloc(GetSizeInBytesFromGLDataType(type) * size) });
				//printf("Uniform #%d Type: %u Name: %s Size: %d\n", i, type, uniformName, size);
			}
		}
		void Shader::Enable()
		{
			glUseProgram(m_ShaderID);
		}
		void Shader::Disable() const
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
		std::string Shader::ReadFile(std::string absoluteFilePath)
		{
			std::string result;
			std::ifstream in(absoluteFilePath, std::ios::in | std::ios::binary);
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
		GLint Shader::GetUniformLocation(std::string name)
		{
			if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
				return m_UniformLocationCache[name];

			GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
			m_UniformLocationCache[name] = location;
			return location;
		}
		void Shader::UploadUniform(std::string uniformName, size_t size, const void* data)
		{
			if (m_UniformBuffer.find(uniformName) != m_UniformBuffer.end())
			{
				auto iterator = m_UniformBuffer.find(uniformName);
				if ((GetSizeInBytesFromGLDataType(iterator->second.Type) * iterator->second.Count) != size)
				{
					CRITICAL_ASSERT("Memory size you tried to set is not valid.");
				}
				else
				{
					switch (iterator->second.Type)
					{
						case GL_FLOAT:		  UploadFloat(iterator->first, *(float*)data);break;
						case GL_SAMPLER:
						case GL_SAMPLER_2D:
						case GL_SAMPLER_3D:
						case GL_SAMPLER_1D:
						case GL_SAMPLER_CUBE:
						case GL_INT:
						case GL_UNSIGNED_BYTE:
						case GL_UNSIGNED_INT: UploadIntegerArray(iterator->first, (int*)data, iterator->second.Count); break;
						case GL_FLOAT_VEC2:   UploadFloat2(iterator->first, *(glm::vec2*)data); break;
						case GL_FLOAT_VEC3:	  UploadFloat3(iterator->first, *(glm::vec3*)data); break;
						case GL_FLOAT_VEC4:	  UploadFloat4(iterator->first, *(glm::vec4*)data); break;
						case GL_FLOAT_MAT4:	  UploadMat4(iterator->first, *(glm::mat4*)data); break;
					}
				}
			}
#ifdef OLD_SLOW_PATH
			std::vector<UniformData>::iterator it;
			for (it = m_UniformBuffer.begin(); it != m_UniformBuffer.end(); it++)
			{
				if (it->Name == uniformName)
				{
					//This is enormously dangerous. You need a safe_check here before you do this assignment.
					if ((GetSizeInBytesFromGLDataType(it->Type) * it->Count) != size)
					{
						CRITICAL_ASSERT("Memory size you tried to set is not valid.");
					}
					else
					{
						memcpy_s(it->Data, GetSizeInBytesFromGLDataType(it->Type) * it->Count, data, size);
						switch (it->Type)
						{
							case GL_FLOAT:		  UploadFloat(it->Name, *(float*)it->Data);                 break;
							case GL_SAMPLER:
							case GL_SAMPLER_2D:
							case GL_SAMPLER_3D:
							case GL_SAMPLER_1D:
							case GL_SAMPLER_CUBE:
							case GL_INT:
							case GL_UNSIGNED_BYTE:
							case GL_UNSIGNED_INT: UploadIntegerArray(it->Name, (int*)it->Data, it->Count);  break;
							case GL_FLOAT_VEC2:   UploadFloat2(it->Name, *(glm::vec2*)it->Data);			break;
							case GL_FLOAT_VEC3:	  UploadFloat3(it->Name, *(glm::vec3*)it->Data);			break;
							case GL_FLOAT_VEC4:	  UploadFloat4(it->Name, *(glm::vec4*)it->Data);			break;
							case GL_FLOAT_MAT4:	  UploadMat4  (it->Name, *(glm::mat4*)it->Data);			break;
						}
					}
				}
			}
#endif
		}
		void Shader::UploadFloat(const std::string name, const float value)
		{
			Enable();
			glUniform1f(GetUniformLocation(name), value);
		}
		void Shader::UploadFloatArray(const std::string name, float* value, int count)
		{
			Enable();
			glUniform1fv(GetUniformLocation(name), count, value);
		}
		void Shader::UploadInteger(const std::string name, const int value)
		{
			Enable();
			glUniform1i(GetUniformLocation(name), value);
		}
		void Shader::UploadFloat2(const std::string name, const glm::vec2& vector)
		{
			Enable();
			glUniform2f(GetUniformLocation(name), vector.x, vector.y);
		}
		void Shader::UploadFloat3(const std::string name, const glm::vec3& vector)
		{
			Enable();
			glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z);
		}
		void Shader::UploadFloat4(const std::string name, const glm::vec4& vector)
		{
			Enable();
			glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
		}
		void Shader::UploadMat4(const std::string name, const glm::mat4& matrix)
		{
			Enable();
			glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
		}
		void Shader::UploadIntegerArray(const std::string name, int* values, uint32_t count)
		{
			Enable();
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