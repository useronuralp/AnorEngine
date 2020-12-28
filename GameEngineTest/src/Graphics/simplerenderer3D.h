#pragma once
#include "renderable3D.h"
#include "shader.h"

namespace GameEngineTest {
	namespace Graphics {

		class Renderer3D
		{
			int modelLocation;
			int viewLocation;  //these variables grab the locaiton of corresponding matrices from the shader.
			int projectionLocation;
		public:
			void instancedDraw(Renderable3D& object, Shader& shader, const glm::mat4& camera, const int& vertexCount, const int& repeatAmount);
			void singleDraw(Renderable3D& object, Shader& shader, const glm::mat4& camera, const int& vertexCount);

			//TO DO: implement indexed drawing.
		private:
			void updateMVP(Renderable3D& object, Shader& shader, const glm::mat4& camera);
		};
	}
}
