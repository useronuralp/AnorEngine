#include "pch.h"
#include "simpleRenderer2D.h"
#include "renderer2D.h"
#include <time.h>
namespace AnorEngine {
	namespace Graphics {
		
		void SimpleRenderer2D::submit(const Renderable2D* sprite) // change this back to Renderable2D later.
		{
			m_RenderQueue.push_back((StaticSprite*) sprite); //cast renderable to (StaticSprite*) later.
		}
		void SimpleRenderer2D::flush()
		{	
			float i = 0.1f;
			while (!m_RenderQueue.empty())
			{
				const StaticSprite* sprite = m_RenderQueue.front();
				sprite->getVAO()->bind();
				sprite->getIBO()->bind();
				sprite->getShader().enable();
				
				//sprite->getShader().setUniformMat4("ml_matrix", Math::mat4::translation(sprite->getPosition()));;
				//sprite->getShader().setUniform4f("colour", sprite->getColor());

				//glDrawElements(GL_TRIANGLES, sprite->getIBO()->getCount(), GL_UNSIGNED_SHORT, nullptr);

				sprite->getIBO()->unbind();
				sprite->getVAO()->unbind();

				m_RenderQueue.pop_front();			
			}
			
		}
		void SimpleRenderer2D::end()
		{
		}
		void SimpleRenderer2D::begin()
		{
		}
	}
}