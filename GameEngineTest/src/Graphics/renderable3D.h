#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <algorithm> 



namespace GameEngineTest {
	namespace Graphics {

		constexpr auto POSITION_VALUE = 0;
		constexpr auto UV_VALUE = 1;
		constexpr auto TID_VALUE = 2;			//layout values, must be matching with the shader values.
		constexpr auto COLOR_VALUE = 3;
		constexpr auto NORMAL_VALUE = 4;
		class Renderable3D
		{
		public:
			glm::mat4 modelMatrix;
			glm::mat4 viewMatrix;
			glm::mat4 projectionMatrix;
		private:
			GLuint m_VBO;
			GLuint m_VAO;
		public:
			Renderable3D(const float* vertex, const int& vertexComponentCount, int* layout, const int& layoutComponentCount, const int& stride);
		public:
			void rotate(const float& degree, const float& x, const float& y, const float& z);
			void translate(const float& x, const float& y, const float& z);
			void scale(const float& x, const float& y, const float& z);
		public:
			void bindVAO();
			void unbindVAO();
			void bindVBO();
			void unbindVBO();
		};
	}
}