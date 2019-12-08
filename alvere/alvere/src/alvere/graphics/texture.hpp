#pragma once

#include <string>

#include "alvere/assets.hpp"
#include "alvere/math/vectors.hpp"
#include "alvere/utils/shapes.hpp"

namespace alvere
{
	class Texture
	{
	public:
		enum class Channels
		{
			Grey = 1,
			GreyAlpha = 2,
			RGB = 3,
			RGBAlpha = 4
		};

		static Asset<Texture> New(const char * filename, Channels channels = Channels::RGBAlpha);

		static Asset<Texture> New(const unsigned char * data, int width, int height, Channels channels = Channels::RGBAlpha);

		static Asset<Texture> New(const Texture & sourceTexture, alvere::RectI sourceRect);

		static Texture * loadFromFile(const std::string & filepath);

		virtual ~Texture();

		int width() const;

		int height() const;

		int channelCount() const;

		unsigned char * pixelData() const;

		RectI getBounds() const;

		alvere::Vector2 texCoords(const alvere::Vector2i& texPosition) const;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		virtual void * getHandle() const = 0;

	protected:

		int m_resWidth;

		int m_resHeight;

		int m_channelCount;

		unsigned char * m_pixelData;

		Texture(const char * filename, Channels channels = Channels::RGBAlpha);

		Texture(const unsigned char * data, int width, int height, Channels channels = Channels::RGBAlpha);

		Texture(const Texture & sourceTexture, alvere::RectI sourceRect);
	};
}