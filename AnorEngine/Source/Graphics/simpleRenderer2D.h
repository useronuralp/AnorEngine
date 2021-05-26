#pragma once
#include "renderer2D.h"
#include "static_sprite.h"

namespace AnorEngine {
	namespace Graphics {

		class ANOR_API SimpleRenderer2D : public Renderer2D
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