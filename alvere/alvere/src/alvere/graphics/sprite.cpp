#include "alvere/graphics/sprite.hpp"

#include "alvere/graphics/texture.hpp"

namespace alvere
{
	Sprite::Sprite()
		: m_texture(nullptr), m_bounds(), m_textureSource(), m_colour(Vector4::unit)
	{ }

	Sprite::Sprite(const Texture & texture, Rect bounds, RectI textureSource, Vector4 colour)
		: m_texture(&texture), m_bounds(bounds), m_textureSource(textureSource), m_colour(colour)
	{ }

	Sprite::Sprite(const Texture & texture, Vector2 position, Vector4 colour)
		: Sprite(texture, Rect{ position.x, position.y, (float)texture.width(), (float)texture.height() }, texture.getBounds(), colour)
	{ }

	Sprite::Sprite(const Texture & texture, Vector2 position, Vector2 scale, Vector4 colour)
		: Sprite(texture, Rect{ position.x, position.y, 0, 0 }, texture.getBounds(), colour)
	{
		setScale(scale);
	}

	Sprite::Sprite(const Texture & texture, Vector2 position, Vector2 scale, RectI textureSource, Vector4 colour)
		: Sprite(texture, Rect{ position.x, position.y, 0, 0 }, textureSource, colour)
	{
		setScale(scale);
	}

	Sprite::Sprite(const Texture & texture, Vector2 position, RectI textureSource, Vector4 colour)
		: Sprite(texture, Rect{ position.x, position.y, (float)textureSource.m_width, (float)textureSource.m_height }, textureSource, colour)
	{ }
}