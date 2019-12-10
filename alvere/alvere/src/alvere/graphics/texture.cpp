#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERIMG
#include <stb/image.hpp>

#include "alvere/graphics/texture.hpp"
#include "alvere/utils/logging.hpp"

namespace alvere
{
	Texture::~Texture()
	{
		stbi_image_free(m_pixelData);
	}

	Vec2i Texture::getDimensions() const
	{
		return m_dimensions;
	}

	int Texture::channelCount() const
	{
		return m_channelCount;
	}

	unsigned char * Texture::pixelData() const
	{
		return m_pixelData;
	}

	RectI Texture::getBounds() const
	{
		return RectI{ 0, 0, m_dimensions.x, m_dimensions.y };
	}

	alvere::Vector2 Texture::texCoords(const alvere::Vector2i& texPosition) const
	{
		return alvere::Vector2 {
			(float)texPosition[0] / m_dimensions.x,
			(float)texPosition[1] / m_dimensions.y
		};
	}

	Texture::Texture(const char * filename, Channels channels)
	{
#ifdef ALV_GRAPHICS_API_OPENGL
		stbi_set_flip_vertically_on_load(true);
#endif

		m_pixelData = stbi_load(filename, &m_dimensions.x, &m_dimensions.y, &m_channelCount, (int)channels);

		if (m_pixelData == nullptr)
		{
			LogError("Failed to load image '%s': %s\n", filename, stbi_failure_reason());
		}
	}

	Texture::Texture(const unsigned char * data, int width, int height, Channels channels)
		: m_dimensions(width, height), m_channelCount((int)channels)
	{
		m_pixelData = (unsigned char *)std::malloc((size_t)m_dimensions.x * m_dimensions.y * m_channelCount);

		if (m_pixelData == nullptr)
		{
			m_dimensions.x = 0;
			m_dimensions.y = 0;
			m_channelCount = 0;
			return;
		}

		std::memcpy(m_pixelData, data, (size_t)m_dimensions.x * m_dimensions.y * m_channelCount);
	}

	Texture::Texture(const Texture & sourceTexture, alvere::RectI sourceRect)
		: m_dimensions(sourceRect.m_width, sourceRect.m_height), m_channelCount(sourceTexture.m_channelCount)
	{
		m_pixelData = (unsigned char *) std::malloc((size_t)m_dimensions.x * m_dimensions.y * m_channelCount);

		if (m_pixelData == nullptr)
		{
			m_dimensions.x = 0;
			m_dimensions.y = 0;
			m_channelCount = 0;
			return;
		}

		for (size_t y = 0; y < sourceRect.m_height; ++y)
		{
			size_t sourceOffset = sourceRect.m_x + (sourceRect.m_y + y) * sourceTexture.m_dimensions.x;
			sourceOffset *= m_channelCount;

			size_t destOffset = y * (size_t) sourceRect.m_width;
			destOffset *= m_channelCount;

			size_t stride = sourceRect.m_width;
			stride *= m_channelCount;

			std::memcpy(m_pixelData + destOffset, sourceTexture.m_pixelData + sourceOffset, stride);
		}
	}

	Texture::Texture(int width, int height, Channels channels)
		: m_dimensions(width, height), m_channelCount((int)channels)
	{
		m_pixelData = (unsigned char *)std::calloc(m_dimensions.x * m_dimensions.y, m_channelCount);

		if(m_pixelData == nullptr)
		{
			m_dimensions.x = m_dimensions.y = m_channelCount = 0;
		}
	}
}