#pragma once

#include "graphics_api/opengl/opengl_texture.hpp"

#include <glad/glad.h>

#include "graphics_api/opengl/opengl_errors.hpp"

namespace alvere::graphics_api::opengl
{
	Texture::Texture(const char * filename, Channels channels)
		: alvere::Texture(filename, channels)
	{
		Init();
	}

	Texture::Texture(const unsigned char * data, int width, int height, Channels channels)
		: alvere::Texture(data, width, height, channels)
	{
		Init();
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

	void Texture::Init()
	{
		GLenum format = GL_RGBA;

		switch (m_channelCount)
		{
			case 1: format = GL_RED; break;
			case 2: format = GL_RG; break;
			case 3: format = GL_RGB; break;
			case 4: format = GL_RGBA; break;
		}

		GLenum internalFormat = GL_RGBA8;

		switch (m_channelCount)
		{
			case 1: internalFormat = GL_R8; break;
			case 2: internalFormat = GL_RG8; break;
			case 3: internalFormat = GL_RGB8; break;
			case 4: internalFormat = GL_RGBA8; break;
		}

		glGenTextures(1, &m_Handle);
		glBindTexture(GL_TEXTURE_2D, m_Handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		ALV_LOG_OPENGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_resWidth, m_resHeight, 0, format, GL_UNSIGNED_BYTE, m_pixelData));
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

alvere::Asset<alvere::Texture> alvere::Texture::New(const char * filename, Channels channels)
{
	return alvere::Asset<alvere::Texture>(new graphics_api::opengl::Texture(filename, channels));
}

alvere::Asset<alvere::Texture> alvere::Texture::New(const unsigned char * data, int width, int height, Channels channels)
{
	return alvere::Asset<alvere::Texture>(new graphics_api::opengl::Texture(data, width, height, channels));
}