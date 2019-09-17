#pragma once

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

		virtual ~Texture();

		int width() const;

		int height() const;

		int channelCount() const;

		unsigned char * pixelData() const;

		RectI bounds() const;

		alvere::Vector2 texCoords(const alvere::Vector2i& texPosition) const;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

	protected:

		int m_resWidth;

		int m_resHeight;

		int m_channelCount;

		unsigned char * m_pixelData;

		Texture(const char * filename, Channels channels = Channels::RGBAlpha);
	};
}