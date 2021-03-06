#pragma once

#include "alvere/graphics/texture.hpp"

namespace alvere::graphics_api::opengl
{
	class Texture : public alvere::Texture
	{
	public:
		Texture(const char * filename, Channels channels = Channels::RGBAlpha);

		Texture(const unsigned char * data, int width, int height, Channels channels = Channels::RGBAlpha);

		Texture(int width, int height, Channels channels = Channels::RGBAlpha);

		Texture(const alvere::Texture & sourceTexture, alvere::RectI sourceRect);

		~Texture();

		void bind() const override;

		void unbind() const override;

		void * getHandle() const override;

		bool resize(unsigned int width, unsigned int height) override;

	private:

		unsigned int m_Handle;

		unsigned int m_internalFormat;
			
		unsigned int m_format;

		void init();
	};
}