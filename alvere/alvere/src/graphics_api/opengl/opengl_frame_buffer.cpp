#include "graphics_api/opengl/opengl_frame_buffer.hpp"

#include <glad/glad.h>

#include "alvere/utils/logging.hpp"

namespace alvere::graphics_api::opengl
{
	FrameBuffer::FrameBuffer()
		: ::alvere::FrameBuffer(), m_handle(0), m_textureHandle(0), m_depthStencilBufferHandle(0)
	{}

	FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
		: ::alvere::FrameBuffer(width, height), m_handle(0), m_textureHandle(0), m_depthStencilBufferHandle(0)
	{
		glGenFramebuffers(1, &m_handle);
		glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

		m_texture = Texture::New(width, height, Texture::Channels::RGB);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (unsigned int)m_texture->getHandle(), 0);

		glGenRenderbuffers(1, &m_depthStencilBufferHandle);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBufferHandle);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBufferHandle);

		unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		switch(status)
		{
			case GL_FRAMEBUFFER_COMPLETE: break;
			case GL_FRAMEBUFFER_UNDEFINED: LogError("Incomplete OpenGL FrameBuffer creation: The specified FrameBuffer is the default read or draw FrameBuffer, but the default FrameBuffer does not exist."); break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: LogError("Incomplete OpenGL FrameBuffer creation: One or more of the FrameBuffer attachment points are FrameBuffer incomplete."); break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: LogError("Incomplete OpenGL FrameBuffer creation: The FrameBuffer does not have at least one image attached to it."); break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: LogError("Incomplete OpenGL FrameBuffer creation: The value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for one or more of the colour attachment point(s) named by GL_DRAW_BUFFERi."); break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: LogError("Incomplete OpenGL FrameBuffer creation: GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the colour attachment point named by GL_READ_BUFFER."); break;
			case GL_FRAMEBUFFER_UNSUPPORTED: LogError("Incomplete OpenGL FrameBuffer creation: The combination of the internal formats of the attached images violates an implementation-dependent set of restrictions."); break;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: LogError("Incomplete OpenGL FrameBuffer creation: either the value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; The attached images are a mix of renderbuffers and textures, and the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES; the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures; or, the attached images are a mix of renderbuffers and textures, and the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures."); break;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: LogError("Incomplete OpenGL FrameBuffer creation: One or more FrameBuffer attachment is layered, and one or more populated attachment is not layered, or all populated colour attachments are not from textures of the same target."); break;
		}
	}

	FrameBuffer::FrameBuffer(const FrameBuffer& frameBuffer)
	{
		*this = frameBuffer;
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_handle);
	}

	FrameBuffer& FrameBuffer::operator=(const FrameBuffer& frameBuffer)
	{
		return *this;
	}

	void FrameBuffer::setWidth(unsigned int newWidth)
	{
		if (newWidth == m_width)
		{
			return;
		}

		m_width = newWidth;
	}

	void FrameBuffer::setHeight(unsigned int newHeight)
	{
		if (newHeight == m_height)
		{
			return;
		}

		m_height = newHeight;
	}

	void FrameBuffer::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
	}

	void FrameBuffer::unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

std::unique_ptr<alvere::FrameBuffer> alvere::FrameBuffer::create(unsigned int width, unsigned int height)
{
	return std::make_unique<alvere::graphics_api::opengl::FrameBuffer>(width, height);
}