#include <GL/glew.h>
#include <glm.hpp>
namespace AnorEngine
{
	namespace Graphics
	{
		enum class FramebufferTextureFormat
		{
			None = 0,
			//Color
			RGBA8 = 1,
			RGBA16F = 2,
			RGBA32F = 3,
			RG32F = 4,

			//Depth / Stencil
			DEPTH32F = 5,
			DEPTH24STENCIL8 = 6,

			//Defaults
			Depth = DEPTH24STENCIL8
		};

		struct FramebufferTextureSpecification
		{
			FramebufferTextureSpecification() = default;
			FramebufferTextureSpecification(FramebufferTextureFormat format)
				:TextureFormat(format) {}
			FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		};

		struct FramebufferAttachmentSpecification
		{
			FramebufferAttachmentSpecification() = default;
			FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecification>& attachments)
				:Attachments(attachments) {}

			std::vector<FramebufferTextureSpecification> Attachments;
		};
		struct FramebufferSpecifications
		{
			std::vector<FramebufferAttachmentSpecification> Attachments;
			uint32_t FramebufferID = 0;
			uint32_t Width = 1280, Height = 720;

			int Samples = 1;
		};

		class ANOR_API Framebuffer
		{
		private:
			FramebufferSpecifications m_Specs;

			std::vector<FramebufferTextureSpecification> m_ColorAttachmentsSpecs;
			FramebufferTextureSpecification m_DepthAttachmentSpec;

			std::vector<uint32_t> m_ColorAttachments;
			uint32_t m_DepthAttachment;
		public:
			Framebuffer(const FramebufferSpecifications& specs);
			~Framebuffer();
		public:
			void Resize(uint32_t width, uint32_t height);
			glm::vec2 GetDimensions() { return { m_Specs.Width, m_Specs.Height }; }
			void Bind();
			void Unbind();
			inline uint32_t GetColorAttachmentID(uint32_t index = 0) const { return m_ColorAttachments[index]; }
		private:
			void SetupFramebuffer();
		};
	}
}