#pragma once
#include "../renderable2d.h"
#include "../renderer2d.h"
#include "Layer.h"
namespace AnorEngine {
	namespace Graphics {

		class ANOR_API TwoDimensionalLayer : public Layer
		{
		protected:
			Renderer2D *m_Renderer;
			std::vector<Renderable2D*> m_Renderables;
			Shader* m_Shader;
			glm::mat4 m_ProjectionMatrix;
		protected:
			TwoDimensionalLayer(Renderer2D *renderer, Shader *shader, const glm::mat4& projectionMatrix);
		public:
			~TwoDimensionalLayer();
			void add(Renderable2D* renderable);
			void render();
		};
	}
}