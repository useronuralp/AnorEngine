#pragma once
#include <GL/glew.h>
namespace GameEngineTest {
	namespace Graphics {

		enum class ENGINE_API ShaderDataType
		{
			None = 0, vec, vec2, vec3, vec4, mat3, mat4, Int, Int2, Int3, Int4, Bool
		};
		static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::vec:  return GL_FLOAT;
			case ShaderDataType::vec2: return GL_FLOAT;
			case ShaderDataType::vec3: return GL_FLOAT;
			case ShaderDataType::vec4: return GL_FLOAT;
			case ShaderDataType::mat3: return GL_FLOAT;
			case ShaderDataType::mat4: return GL_FLOAT;
			case ShaderDataType::Int:  return GL_INT;
			case ShaderDataType::Int2: return GL_INT;
			case ShaderDataType::Int3: return GL_INT;
			case ShaderDataType::Int4: return GL_INT;
			case ShaderDataType::Bool: return GL_BOOL;
			}
			CRITICAL("Unknown ShaderDataType");
			return 0;
		}
		static uint32_t ShaderDataTypeConverter(ShaderDataType type)
		{
			switch(type)
			{
				case ShaderDataType::vec:	return 4;
				case ShaderDataType::vec2:	return 4 * 2;
				case ShaderDataType::vec3:	return 4 * 3;
				case ShaderDataType::vec4:	return 4 * 4;
				case ShaderDataType::mat3:	return 4 * 3 * 3;
				case ShaderDataType::mat4:	return 4 * 4 * 4;
				case ShaderDataType::Int:	return 4;
				case ShaderDataType::Int2:	return 4 * 2;
				case ShaderDataType::Int3:	return 4 * 3;
				case ShaderDataType::Int4:	return 4 * 4;
				case ShaderDataType::Bool:	return 1;
			}

			CRITICAL("Unknown ShaderDataType!!");
			__debugbreak;
			return 0;
		}
		struct ENGINE_API BufferElement
		{
			std::string name;
			ShaderDataType type;
			uint32_t size;
			uint32_t offset;
			uint32_t componentCount = 0;
			uint32_t ShaderLayoutLocation; // used to match the shader layout (location = ?) value.
			bool normalized;
		public:
			//BufferElement() {}
			BufferElement(ShaderDataType type, std::string name, int shaderLocation, bool normalized = false)
				:name(name), type(type), size(ShaderDataTypeConverter(type)), offset(0), ShaderLayoutLocation(shaderLocation), normalized(normalized)
			{
				switch (type)
				{
					case ShaderDataType::vec:	componentCount = 1;		break;
					case ShaderDataType::vec2:	componentCount = 2;		break;
					case ShaderDataType::vec3:	componentCount = 3;		break;
					case ShaderDataType::vec4:	componentCount = 4;		break;
					case ShaderDataType::mat3:	componentCount = 3 * 3;	break;
					case ShaderDataType::mat4:	componentCount = 4 * 4;	break;
					case ShaderDataType::Int:	componentCount = 1;		break;
					case ShaderDataType::Int2:	componentCount = 2;		break;
					case ShaderDataType::Int3:	componentCount = 3;		break;
					case ShaderDataType::Int4:	componentCount = 4;		break;
					case ShaderDataType::Bool:	componentCount = 1;		break;
				}
			}
		};

		class ENGINE_API BufferLayout
		{
		private:
			std::vector<BufferElement> m_Elements;
			uint32_t m_Stride = 0;
			uint32_t totalComponentCount = 0;
		public:
			BufferLayout() {}
			BufferLayout(const std::initializer_list<BufferElement>& elements)
				:m_Elements(elements)
			{
				CalculateOffsetAndStride();
			}
		private:
			void CalculateOffsetAndStride()
			{
				uint32_t offset = 0;
				m_Stride = 0;
				for (auto& element : m_Elements)
				{
					element.offset = offset;
					offset += element.size;
					totalComponentCount += element.componentCount;
					m_Stride += element.size;
				}
			}		
		public:	
			inline const std::vector<BufferElement>& GetBufferElements() const { return m_Elements; }
			uint32_t getStride() { return m_Stride; }
			uint32_t getComponentCount() { return totalComponentCount; }
		};

		class ENGINE_API Buffer
		{
			friend class VertexArray;
		private:
			uint32_t m_Size;
			GLuint m_BufferID;
			BufferLayout m_Layout;
		public:
			Buffer(GLfloat * vertices, uint32_t size, BufferLayout layout); // size = total component count * sizeof(passed variable)
			~Buffer();
			void bind() const;
			void unbind() const;
			inline BufferLayout& getBufferLayout() { return m_Layout; }
			//void SetLayout(BufferLayout& layout) { m_Layout = layout; }
		};

		class ENGINE_API IndexBuffer
		{
			friend class VertexArray;
		private:
			GLuint m_BufferID;
			uint32_t m_Count;
		private:
			IndexBuffer()
				:m_BufferID(0), m_Count(0){}
		public:
			IndexBuffer(uint32_t* data, uint32_t count);
			~IndexBuffer();
			void bind() const;
			void unbind() const;
		public:
			uint32_t GetCount() { return m_Count; }
		};
	}
}