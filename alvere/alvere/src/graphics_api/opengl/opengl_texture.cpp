#pragma once

#include "graphics_api/opengl/opengl_texture.hpp"

#include <glad/glad.h>

#include "graphics_api/opengl/opengl_errors.hpp"

namespace alvere::graphics_api::opengl
{
	Texture::Texture(const char * filename, Channels channels)
		: alvere::Texture(filename, channels)
	{
		glGenTextures(1, &m_Handle);
		glBindTexture(GL_TEXTURE_2D, m_Handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		ALV_LOG_OPENGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_resWidth, m_resHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixelData));
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_Handle);
	}

	void Texture::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_Handle);
	}

	void Texture::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

alvere::Asset<alvere::Texture> alvere::Texture::New(const char * filename, Channels channels)
{
	return alvere::Asset<alvere::Texture>(new graphics_api::opengl::Texture(filename, channels));
}