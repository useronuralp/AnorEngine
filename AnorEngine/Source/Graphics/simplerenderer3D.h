#pragma once
#include "renderable3D.h"
#include "shader.h"
#include "Camera.h"
namespace AnorEngine {
	namespace Graphics {

		class ANOR_API Renderer3D
		{
			int m_ShaderModelLocation;
			int m_ShaderViewLocation;  //these variables grab the locaiton of corresponding matrices from the shader.
			int m_ShaderProjectionLocation;
		public:
			void instancedDraw(Renderable3D& object, Shader& shader, const glm::mat4& camera, const int& vertexCount, const int& repeatAmount);
			void singleDraw(Ref<Renderable3D> object, Ref<Shader> shader, Ref<PerspectiveCamera> camera, const int& vertexCount);

			//TO DO: implement indexed drawing.
		private:
			void updateMVP(Ref<Renderable3D> object, Ref<Shader> shader, Ref<PerspectiveCamera> camera);
		};
	}
}
