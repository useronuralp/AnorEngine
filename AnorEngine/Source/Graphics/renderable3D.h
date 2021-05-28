#pragma once
#include "Buffers/VertexArray.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
namespace AnorEngine {
	namespace Graphics {

		constexpr auto POSITION_VALUE = 0;
		constexpr auto UV_VALUE = 1;
		constexpr auto TID_VALUE = 2;			//layout values, must be matching with the shader values. (Currently not in use, using BufferLayout class instead)
		constexpr auto COLOR_VALUE = 3;
		constexpr auto NORMAL_VALUE = 4;

		struct ANOR_API VertexInfo {
			glm::vec3 Position;
			glm::vec2 TexCoords;
			glm::vec3 Normal;
		};

		class ANOR_API Renderable3D
		{
		public:
			glm::mat4 m_ModelMatrix;
		private:
			VertexArray m_VAO;
		public:
			Renderable3D(Ref<Buffer> buffer);
		public:
			void rotate(const float& degree, const float& x, const float& y, const float& z);
			void translate(const float& x, const float& y, const float& z);
			void scale(const float& x, const float& y, const float& z);
		public:
			void bindVAO();
			void unbindVAO();
			const VertexArray& getVertexArray() const { return m_VAO; }
		private:
			void init(Ref<Buffer> buffer);
		};
	}
}