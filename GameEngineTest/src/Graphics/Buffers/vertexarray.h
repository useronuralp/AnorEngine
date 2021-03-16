#pragma once
#include <GL/glew.h>
#include "buffer.h"
namespace GameEngineTest {
	namespace  Graphics {

		class ENGINE_API VertexArray
		{
		private:
			GLuint m_ArrayID;
		public:
			VertexArray();
			~VertexArray();

			void bindBuffer(Buffer *buffer, GLuint index);
			void bind() const;
			void unbind() const;
		};
	}
}
