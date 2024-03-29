#pragma once
#include <glad/glad.h>
namespace AnorEngine {
	namespace Graphics {

		enum class ANOR_API ShaderDataType
		{
			None = 0, vec, vec2, vec3, vec4, mat3, mat4, Int, Int2, Int3, Int4, Bool
		};
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

			CRITICAL_ASSERT("Unknown ShaderDataType!!");
			return 0;
		}

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
			CRITICAL_ASSERT("Unknown ShaderDataType");
			return 0;
		}

		struct ANOR_API BufferElement
		{
			std::string Name;
			ShaderDataType Type;
			uint32_t SizeByte;
			uint32_t Offset;
			uint32_t ComponentCount = 0;
			uint32_t ShaderLayoutLocation; // used to match the shader layout (location = ?) value.
			bool Normalized;

			BufferElement(ShaderDataType type, std::string name, int shaderLocation, bool normalized = false)
				:Name(name), Type(type), SizeByte(ShaderDataTypeConverter(type)), Offset(0), ShaderLayoutLocation(shaderLocation), Normalized(normalized)
			{
				switch (type)
				{
					case ShaderDataType::vec:	ComponentCount = 1;		break;
					case ShaderDataType::vec2:	ComponentCount = 2;		break;
					case ShaderDataType::vec3:	ComponentCount = 3;		break;
					case ShaderDataType::vec4:	ComponentCount = 4;		break;
					case ShaderDataType::mat3:	ComponentCount = 3 * 3;	break;
					case ShaderDataType::mat4:	ComponentCount = 4 * 4;	break;
					case ShaderDataType::Int:	ComponentCount = 1;		break;
					case ShaderDataType::Int2:	ComponentCount = 2;		break;
					case ShaderDataType::Int3:	ComponentCount = 3;		break;
					case ShaderDataType::Int4:	ComponentCount = 4;		break;
					case ShaderDataType::Bool:	ComponentCount = 1;		break;
				}
			}
		};

		class ANOR_API BufferLayout
		{
		private:
			std::vector<BufferElement> m_Elements;
			uint32_t m_StrideByte = 0;
			uint32_t m_TotalComponentCount = 0;
		public:
			std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
			std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
			const std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
			const std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
		public:
			BufferLayout(const std::initializer_list<BufferElement>& elements)
				:m_Elements(elements)
			{
				CalculateOffsetAndStride();
			}
		private:
			void CalculateOffsetAndStride()
			{
				uint32_t offset = 0;
				m_StrideByte = 0;
				for (auto& element : m_Elements)
				{
					element.Offset = offset;
					offset += element.SizeByte;
					m_TotalComponentCount += element.ComponentCount;
					m_StrideByte += element.SizeByte;
				}
			}		
		public:	
			inline const std::vector<BufferElement>& GetLayoutElements() const { return m_Elements; }
			const uint32_t& GetStride() const { return m_StrideByte; }
			const uint32_t& GetComponentCount() const { return m_TotalComponentCount; }
		};

		class ANOR_API VertexBuffer
		{
		private:
			uint32_t m_SizeByte;
			uint32_t m_BufferID;
			BufferLayout m_Layout;
		public:
			VertexBuffer(uint32_t sizeByte, BufferLayout& layout);
			VertexBuffer(float* vertices, uint32_t sizeByte, BufferLayout& layout); // size = total component count * sizeof(passed variable type)
			~VertexBuffer();
			void SetData(const void* data, uint32_t dataSize);
			void Bind() const;
			void Unbind() const;
			const inline BufferLayout& GetBufferLayout() const { return m_Layout; }
			//void SetLayout(BufferLayout& layout) { m_Layout = layout; }
		};

		class ANOR_API IndexBuffer
		{
		private:
			uint32_t m_BufferID;
			uint32_t m_Count;
		private:
			IndexBuffer()
				:m_BufferID(0), m_Count(0){}
		public:
			IndexBuffer(uint32_t* data, uint32_t count);
			~IndexBuffer();
			void Bind() const;
			void Unbind() const;
		public:
			const uint32_t& GetCount() const { return m_Count; }
		};
	}
}