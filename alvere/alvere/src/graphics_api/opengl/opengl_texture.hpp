#pragma once

#include "alvere/graphics/texture.hpp"

namespace alvere::graphics_api::opengl
{
	class Texture : public alvere::Texture
	{
	public:
		Texture(const char * filename, Channels channels = Channels::RGBAlpha);

		Texture(const unsigned char * data, int width, int height, Channels channels = Channels::RGBAlpha);

		~Texture();

		void bind() const override;

		void unbind() const override;

	private:

		unsigned int m_Handle;

		void init();
	};
}