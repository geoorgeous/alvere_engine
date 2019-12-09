#pragma once

#include "graphics_api/opengl/opengl_texture.hpp"

#include <glad/glad.h>

#include "graphics_api/opengl/opengl_errors.hpp"

namespace alvere::graphics_api::opengl
{
	Texture::Texture(const char * filename, Channels channels)
		: alvere::Texture(filename, channels)
	{
		init();
	}

	Texture::Texture(const unsigned char * data, int width, int height, Channels channels)
		: alvere::Texture(data, width, height, channels)
	{
		init();
	}
	
	Texture::Texture(const alvere::Texture & sourceTexture, alvere::RectI sourceRect)
		: alvere::Texture(sourceTexture, sourceRect)
	{
		init();
	}



	Texture::Texture(int width, int height, Channels channels)
		: alvere::Texture(width, height, channels)
	{
		init();
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

	void * Texture::getHandle() const
	{
		return (void *)m_Handle;
	}

	void Texture::resize(unsigned int width, unsigned int height)
	{
		unsigned char * newPixelData = (unsigned char *)std::calloc(width * height, m_channelCount);

		int copyWidth = width > m_resWidth ? m_resWidth : width;

		for(int y = 0; y < height && y < m_resHeight; ++y)
		{
			unsigned int start = y * m_resWidth;

			std::copy(
				m_pixelData + start * m_channelCount,
				m_pixelData + (start + copyWidth) * m_channelCount,
				newPixelData + (y * width) * m_channelCount);
		}

		std::free(m_pixelData);

		m_resWidth = width;
		m_resHeight = height;
		m_pixelData = newPixelData;

		glBindTexture(GL_TEXTURE_2D, m_Handle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		ALV_LOG_OPENGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_resWidth, m_resHeight, 0, m_format, GL_UNSIGNED_BYTE, m_pixelData));
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::init()
	{
		switch (m_channelCount)
		{
			case 1: m_format = GL_RED; break;
			case 2: m_format = GL_RG; break;
			case 3: m_format = GL_RGB; break;
			case 4: m_format = GL_RGBA; break;
		}

		switch (m_channelCount)
		{
			case 1: m_internalFormat = GL_R8; break;
			case 2: m_internalFormat = GL_RG8; break;
			case 3: m_internalFormat = GL_RGB8; break;
			case 4: m_internalFormat = GL_RGBA8; break;
		}

		glGenTextures(1, &m_Handle);
		glBindTexture(GL_TEXTURE_2D, m_Handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		ALV_LOG_OPENGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_resWidth, m_resHeight, 0, m_format, GL_UNSIGNED_BYTE, m_pixelData));
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
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

alvere::Asset<alvere::Texture> alvere::Texture::New(int width, int height, Channels channels)
{
	return alvere::Asset<alvere::Texture>(new graphics_api::opengl::Texture(width, height, channels));
}

alvere::Texture * alvere::Texture::loadFromFile(const std::string & filepath)
{
	return new graphics_api::opengl::Texture(filepath.c_str());
}

alvere::Asset<alvere::Texture> alvere::Texture::New(const alvere::Texture & sourceTexture, alvere::RectI sourceRect)
{
	return alvere::Asset<alvere::Texture>(std::make_unique<graphics_api::opengl::Texture>(sourceTexture, sourceRect));
}
