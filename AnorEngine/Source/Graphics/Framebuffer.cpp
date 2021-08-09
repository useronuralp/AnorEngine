#include "pch.h"
#include "Framebuffer.h"

namespace AnorEngine
{
	namespace Graphics
	{
		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}
		static void CreateTextures(bool multiSample, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(multiSample), count, outID);
		}
		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch(format)
			{
				case FramebufferTextureFormat::DEPTH32F: return true;
				case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
			}
			return false;
		}
		static void BindTexture(bool multiSample, uint32_t id)
		{
			glBindTexture(TextureTarget(multiSample), id);
		}
		static void AttachColorTexture(uint32_t id, int samples, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}
		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}
		Framebuffer::Framebuffer(const FramebufferSpecifications& specs)
			:m_Specs(specs)
		{
			for (auto& attachment : m_Specs.Attachments)
			{
				for(auto& specification : attachment.Attachments)
				{
					if(!IsDepthFormat(specification.TextureFormat))
						m_ColorAttachmentsSpecs.emplace_back(specification);
					else
						m_DepthAttachmentSpec = specification.TextureFormat;
				}
			}
			
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
				glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
				glDeleteTextures(1, &m_DepthAttachment);
				
				m_ColorAttachments.clear();
				m_DepthAttachment = 0;
			}

			glCreateFramebuffers(1, &m_Specs.FramebufferID);
			glBindFramebuffer(GL_FRAMEBUFFER, m_Specs.FramebufferID);

			bool multisample = m_Specs.Samples > 1;

			//Attachments
			if(m_ColorAttachmentsSpecs.size())
			{
				//Avoid unnecessary allocation inside the vector.
				m_ColorAttachments.resize(m_ColorAttachmentsSpecs.size());
				CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());
				for (auto& spec : m_ColorAttachmentsSpecs)
				{
					for(size_t i = 0; i < m_ColorAttachments.size(); i++)
					{
						BindTexture(multisample, m_ColorAttachments[i]);
						switch(m_ColorAttachmentsSpecs[i].TextureFormat)
						{
							case FramebufferTextureFormat::RGBA8:
								AttachColorTexture(m_ColorAttachments[i], m_Specs.Samples, GL_RGBA8, m_Specs.Width, m_Specs.Height, i);
								break;
						}
					}
				}
			}
			
			if (m_DepthAttachmentSpec.TextureFormat != FramebufferTextureFormat::None)
			{
				CreateTextures(multisample, &m_DepthAttachment, 1);
				BindTexture(multisample, m_DepthAttachment);
				switch (m_DepthAttachmentSpec.TextureFormat)
				{
					case FramebufferTextureFormat::DEPTH24STENCIL8:
						AttachDepthTexture(m_DepthAttachment, m_Specs.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specs.Width, m_Specs.Height);
						break;
				}
			}
			
			if (m_ColorAttachments.size() > 1)
			{
				GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
				glDrawBuffers(m_ColorAttachments.size(), buffers);
			}
			else if (m_ColorAttachments.empty())
			{
				// Only depth-pass
				glDrawBuffer(GL_NONE);
			}
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				CRITICAL_ASSERT("Framebuffer is incomplete!");
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		}
		Framebuffer::~Framebuffer()
		{
			glDeleteFramebuffers(1, &m_Specs.FramebufferID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);
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