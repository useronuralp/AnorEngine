#include "pch.h"
#include "Framebuffer.h"

namespace AnorEngine
{
	namespace Graphics
	{
		Framebuffer::Framebuffer(const FramebufferSpecifications& specs)
			:m_Specs(specs)
		{
			SetupFramebuffer();
		}
		void Framebuffer::Resize(uint32_t width, uint32_t height)
		{
			m_Specs.Width = width;
			m_Specs.Height = height;
			SetupFramebuffer();
		}
		void Framebuffer::SetupFramebuffer()
		{
			if (m_Specs.FramebufferID) //If the ID is not 0, then it means this framebuffer was already in use and we want to delete it and start the recreation fresh.
			{
				glDeleteFramebuffers(1, &m_Specs.FramebufferID);
				glDeleteTextures(1, &m_Specs.ColorAttachment);
				glDeleteTextures(1, &m_Specs.DepthAttachment);
			}

			glCreateFramebuffers(1, &m_Specs.FramebufferID);
			glBindFramebuffer(GL_FRAMEBUFFER, m_Specs.FramebufferID);

			glCreateTextures(GL_TEXTURE_2D, 1, &m_Specs.ColorAttachment);
			glBindTexture(GL_TEXTURE_2D, m_Specs.ColorAttachment);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specs.Width, m_Specs.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Specs.ColorAttachment, 0);

			glCreateTextures(GL_TEXTURE_2D, 1, &m_Specs.DepthAttachment);
			glBindTexture(GL_TEXTURE_2D, m_Specs.DepthAttachment);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specs.Width, m_Specs.Height);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_Specs.DepthAttachment, 0);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				CRITICAL_ASSERT("Framebuffer is incomplete!");
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		}
		Framebuffer::~Framebuffer()
		{
			glDeleteFramebuffers(1, &m_Specs.FramebufferID);
			glDeleteTextures(1, &m_Specs.ColorAttachment);
			glDeleteTextures(1, &m_Specs.DepthAttachment);
		}
		void Framebuffer::Bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_Specs.FramebufferID);
			glViewport(0, 0, m_Specs.Width, m_Specs.Height);
		}
		void Framebuffer::Unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		
	}
}