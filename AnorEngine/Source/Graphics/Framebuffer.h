#include <GL/glew.h>
#include <glm.hpp>
namespace AnorEngine
{
	namespace Graphics
	{
		struct FramebufferSpecifications
		{
			uint32_t FramebufferID = 0;
			uint32_t DepthAttachment = 0;
			uint32_t ColorAttachment = 0;
			uint32_t Width = 1280, Height = 720;
		};

		class ANOR_API Framebuffer
		{
		private:
			FramebufferSpecifications m_Specs;
		public:
			Framebuffer(const FramebufferSpecifications& specs);
			~Framebuffer();
		public:
			void Resize(uint32_t width, uint32_t height);
			glm::vec2 GetDimensions() { return { m_Specs.Width, m_Specs.Height }; }
			void Bind();
			void Unbind();
			inline uint32_t GetColorAttachmentID() const { return m_Specs.ColorAttachment; }
		private:
			void SetupFramebuffer();
		};
	}
}