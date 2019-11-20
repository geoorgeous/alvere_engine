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

	Sprite::Sprite(const Texture & texture, Rect bounds, Vector4 colour)
		: Sprite(texture, bounds, texture.getBounds(), colour)
	{ }

	Sprite::Sprite(const Texture & texture, Vector4 colour)
		: Sprite(texture, (Rect)texture.getBounds(), texture.getBounds(), colour)
	{ }
}