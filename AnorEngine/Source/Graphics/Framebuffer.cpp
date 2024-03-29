#include "pch.h"
#include "Framebuffer.h"

namespace AnorEngine
{
	namespace Graphics
	{
		
		static GLenum FromAnorFormatToOpenGLFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
				case FramebufferTextureFormat::RED_INTEGER:	return GL_RED_INTEGER; break;
				case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;       break;
				case FramebufferTextureFormat::RGBA16F:     return GL_RGBA16F;     break;
			}
			return GL_INVALID_ENUM;
		}
		static GLenum TextureTarget(bool multisampled, TextureType type)
		{
			switch(type)
			{
				case TextureType::TEXTURE_2D: return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
				case TextureType::CUBEMAP: return GL_TEXTURE_CUBE_MAP;
			}
			
		}
		static void CreateTextures(bool multiSample, TextureType type, uint32_t* outID, uint32_t count)
		{
			//Memory allocation part for the textures.
			glCreateTextures(TextureTarget(multiSample, type), count, outID);
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
		static void BindTexture(bool multiSample, TextureType type, uint32_t id)
		{
			glBindTexture(TextureTarget(multiSample, type), id);
		}
		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, TextureType texType, GLenum type, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			//glFramebufferTexture2D attaches the texture image specified by TEXTURE and LEVEL as one of the logical buffers of the currently bound framebuffer object.
			//Attachment specifies whether the texture image should be attached to the framebuffer object's color, depth, or stencil buffer.
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled , texType), id, 0);
		}
		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, TextureType type, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled && type == TextureType::TEXTURE_2D)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else 
			{
				if (type == TextureType::TEXTURE_2D)
				{
					glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
					float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
					glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
					glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled, type), id, 0);

				}
				else if (type == TextureType::CUBEMAP)
				{
					for (unsigned int i = 0; i < 6; ++i)
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

					glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, id, 0);
				}

			}

		}
		Framebuffer::Framebuffer(const FramebufferSpecifications& specs)
			:m_Specs(specs)
		{
			for (auto& attachmentSpec : m_Specs.Attachments)
			{
				for(auto& textureSpec : attachmentSpec.Attachments)
				{
					if(!IsDepthFormat(textureSpec.TextureFormat))
						m_ColorAttachmentsSpecs.emplace_back(textureSpec);
					else
						m_DepthAttachmentSpec = textureSpec.TextureFormat;
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
				glDeleteTextures(m_ColorAttachmentTextureIDs.size(), m_ColorAttachmentTextureIDs.data());
				glDeleteTextures(1, &m_DepthAttachmentTextureID);
				
				m_ColorAttachmentTextureIDs.clear();
				m_DepthAttachmentTextureID = 0;
			}

			glCreateFramebuffers(1, &m_Specs.FramebufferID);
			glBindFramebuffer(GL_FRAMEBUFFER, m_Specs.FramebufferID);

			bool multisample = m_Specs.Samples > 1;

			//Attachments
			if(m_ColorAttachmentsSpecs.size()) // Check if there is any attachments in here.
			{
				m_ColorAttachmentTextureIDs.resize(m_ColorAttachmentsSpecs.size());
				//Passing a pointer to the empty m_ColorAttachmentTextureIDs to the CreateTextures. Memory block of m_ColorAttachmentTextureIDs will be filled by CreateTextures.
				CreateTextures(multisample, m_Specs.Texture_Type, m_ColorAttachmentTextureIDs.data(), m_ColorAttachmentTextureIDs.size());
				for(size_t i = 0; i < m_ColorAttachmentTextureIDs.size(); i++)
				{
					BindTexture(multisample, m_Specs.Texture_Type, m_ColorAttachmentTextureIDs[i]);
					switch(m_ColorAttachmentsSpecs[i].TextureFormat)
					{
						//This switch is where you extend this code path when there is more types to add.
						case FramebufferTextureFormat::RGBA8:
							AttachColorTexture(m_ColorAttachmentTextureIDs[i], m_Specs.Samples, GL_RGBA8, GL_RGBA, m_Specs.Texture_Type, GL_UNSIGNED_BYTE, m_Specs.Width, m_Specs.Height, i);
							break;
						case FramebufferTextureFormat::RED_INTEGER:
							AttachColorTexture(m_ColorAttachmentTextureIDs[i], m_Specs.Samples, GL_R32I, GL_RED_INTEGER, m_Specs.Texture_Type, GL_INT, m_Specs.Width, m_Specs.Height, i);
							break;
						case FramebufferTextureFormat::RGBA16F:
							AttachColorTexture(m_ColorAttachmentTextureIDs[i], m_Specs.Samples, GL_RGBA16F, GL_RGBA, m_Specs.Texture_Type, GL_FLOAT, m_Specs.Width, m_Specs.Height, i);
							break;
					}
				}
			}
			
			if (m_DepthAttachmentSpec.TextureFormat != FramebufferTextureFormat::None)
			{
				CreateTextures(multisample, m_Specs.Texture_Type, &m_DepthAttachmentTextureID, 1);
				BindTexture(multisample, m_Specs.Texture_Type, m_DepthAttachmentTextureID);
				switch (m_DepthAttachmentSpec.TextureFormat)
				{
					case FramebufferTextureFormat::DEPTH24STENCIL8:
						AttachDepthTexture(m_DepthAttachmentTextureID, m_Specs.Samples, GL_DEPTH24_STENCIL8, m_Specs.Texture_Type, GL_DEPTH_STENCIL_ATTACHMENT, m_Specs.Width, m_Specs.Height);
						break;
				}
			}
			
			if (m_ColorAttachmentTextureIDs.size() > 1)
			{
				//Currently supporting max 4 color attachments.
				GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
				if (m_ColorAttachmentTextureIDs.size() > 4)
				{
					CRITICAL_ASSERT("Buffer size overflowed!");
				}
				//This line of code specifies which attachments to draw to. For example: If you have two color attachments. This function allows you to write to both of them in a fragment shader by specifying 
				// a second layout(location = 1) out vec4 color2; like this.
				
				//The default value for this call is glDrawBuffers(GL_COLOR_ATTACHMENT0); That is why you don't need to specify a color attachment when you draw using only the default one. It calls this function itself.
				glDrawBuffers(m_ColorAttachmentTextureIDs.size(), buffers);
			}
			else if (m_ColorAttachmentTextureIDs.empty())
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
			glDeleteTextures(m_ColorAttachmentTextureIDs.size(), m_ColorAttachmentTextureIDs.data());
			glDeleteTextures(1, &m_DepthAttachmentTextureID);
		}
		void Framebuffer::Bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_Specs.FramebufferID);
			glViewport(0, 0, m_Specs.Width, m_Specs.Height);
		}
		void Framebuffer::ClearTextureAttachmentWithIntegerValue(GLuint colorAttachmentID, int clearValue)
		{
			auto format = FromAnorFormatToOpenGLFormat(m_ColorAttachmentsSpecs[colorAttachmentID].TextureFormat);
			
			//Since the clear value is int in the function paramater, the type is hard coded to GL_INT for now.
			glClearTexImage(m_ColorAttachmentTextureIDs[colorAttachmentID], 0, format, GL_INT, &clearValue);
		}
		void Framebuffer::BindDepthAttachmentTexture()
		{
			switch (m_Specs.Texture_Type)
			{
				case TextureType::TEXTURE_2D:
					glActiveTexture(GL_TEXTURE0 + m_DepthAttachmentTextureID % 32); //allows you to speicfy a texture slot, usually on pc there are 32 texture.
					glBindTexture(GL_TEXTURE_2D, m_DepthAttachmentTextureID);
					break;
				case TextureType::CUBEMAP:
					glActiveTexture(GL_TEXTURE0 + m_DepthAttachmentTextureID % 32); //allows you to speicfy a texture slot, usually on pc there are 32 texture.
					glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthAttachmentTextureID);
					break;
			}
		}
		void Framebuffer::UnbindDepthAttachmentTexture()
		{
			switch (m_Specs.Texture_Type)
			{
			case TextureType::TEXTURE_2D:
				glBindTexture(GL_TEXTURE_2D, 0);
				break;
			case TextureType::CUBEMAP:
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				break;
			}
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		void Framebuffer::Unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		int Framebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
		{
			if (attachmentIndex > m_ColorAttachmentTextureIDs.size())
			{
				CRITICAL_ASSERT("Sizes dont match!");
			}
			glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
			int pixelData;
			glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
			return pixelData;
		}	
	}
}