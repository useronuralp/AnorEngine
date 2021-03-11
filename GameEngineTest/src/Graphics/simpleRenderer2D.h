#pragma once
#include "renderer2D.h"
//#include <deque>//
#include "static_sprite.h"

namespace GameEngineTest {
	namespace Graphics {

		class SimpleRenderer2D : public Renderer2D
		{
		private:
			std::deque<const StaticSprite*> m_RenderQueue;
		public:
			virtual void begin() override;
			virtual void submit(const Renderable2D* renderable) override;
			virtual void flush() override;
			virtual void end() override;
		};
	}
}