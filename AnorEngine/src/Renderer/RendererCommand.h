#pragma once
#include <Graphics/Buffers/VertexArray.h>
#include <glm.hpp>
namespace AnorEngine
{
	namespace Graphics
	{
		class ANOR_API RenderCommand // This class will basically contain all the raw OpenGL commands.
		{
		public:
			static void DrawIndexed(Ref<VertexArray>& vertexArray);
			static void ClearColor(const glm::vec4& color);
			static void Clear();
		};
	}
}