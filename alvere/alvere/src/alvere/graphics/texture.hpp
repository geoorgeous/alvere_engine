#pragma once

#include <memory>
#include <string>

#include "alvere/math/vector/vec_2_i.hpp"
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

		static std::unique_ptr<Texture> New(const char * filename, Channels channels = Channels::RGBAlpha);

		static std::unique_ptr<Texture> New(const unsigned char * data, int width, int height, Channels channels = Channels::RGBAlpha);

		static std::unique_ptr<Texture> New(int width, int height, Channels channels = Channels::RGBAlpha);

		static std::unique_ptr<Texture> New(const Texture & sourceTexture, alvere::RectI sourceRect);

		static Texture * loadFromFile(const std::string & filepath);

		virtual ~Texture();

		Vec2i getDimensions() const;

		int channelCount() const;

		unsigned char * pixelData() const;

		RectI getBounds() const;

		alvere::Vector2 texCoords(const alvere::Vector2i& texPosition) const;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		virtual void * getHandle() const = 0;

		virtual bool resize(unsigned int width, unsigned int height) = 0;

	protected:

		Vec2i m_dimensions;

		int m_channelCount;

		unsigned char * m_pixelData;

		Texture(const char * filename, Channels channels = Channels::RGBAlpha);

		Texture(const unsigned char * data, int width, int height, Channels channels = Channels::RGBAlpha);

		Texture(int width, int height, Channels = Channels::RGBAlpha);

		Texture(const Texture & sourceTexture, alvere::RectI sourceRect);
	};
}