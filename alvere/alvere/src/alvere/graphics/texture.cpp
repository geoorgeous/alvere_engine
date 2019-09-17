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

	int Texture::width() const
	{
		return m_resWidth;
	}

	int Texture::height() const
	{
		return m_resHeight;
	}

	int Texture::channelCount() const
	{
		return m_channelCount;
	}

	unsigned char * Texture::pixelData() const
	{
		return m_pixelData;
	}

	RectI Texture::bounds() const
	{
		return RectI{ 0, 0, m_resWidth, m_resWidth };
	}

	alvere::Vector2 Texture::texCoords(const alvere::Vector2i& texPosition) const
	{
		return alvere::Vector2 {
			(float)texPosition[0] / m_resWidth,
			(float)texPosition[1] / m_resHeight
		};
	}

	Texture::Texture(const char * filename, Channels channels)
	{
#ifdef ALV_GRAPHICS_API_OPENGL
		stbi_set_flip_vertically_on_load(true);
#endif

		m_pixelData = stbi_load(filename, &m_resWidth, &m_resHeight, &m_channelCount, (int)channels);

		if (m_pixelData == nullptr)
		{
			LogError("Failed to load image '%s': %s\n", filename, stbi_failure_reason());
		}
	}
}