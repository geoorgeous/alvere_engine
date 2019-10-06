#pragma once

#include "alvere/graphics/texture.hpp"
#include "alvere/math/vector/vector_2.hpp"
#include "alvere/math/vector/vector_3.hpp"
#include "alvere/utils/shapes.hpp"

namespace alvere
{
	class Texture;

	class Sprite
	{
	public:

		Sprite();

		Sprite(const Texture & texture, Rect bounds, RectI textureSource, Vector4 colour = Vector4::unit);

		Sprite(const Texture & texture, Vector2 position, Vector4 colour = Vector4::unit);

		Sprite(const Texture & texture, Vector2 position, Vector2 scale, Vector4 colour = Vector4::unit);

		Sprite(const Texture & texture, Vector2 position, Vector2 scale, RectI textureSource, Vector4 colour = Vector4::unit);

		Sprite(const Texture & texture, Vector2 position, RectI textureSource, Vector4 colour = Vector4::unit);

		inline const Texture & getTexture() const
		{
			return *m_texture;
		}

		inline void setTexture(const Texture & texture)
		{
			m_texture = &texture;
		}

		inline const Rect & bounds() const
		{
			return m_bounds;
		}

		inline Rect & bounds()
		{
			return m_bounds;
		}

		inline const RectI & textureSource() const
		{
			return m_textureSource;
		}

		inline RectI & textureSource()
		{
			return m_textureSource;
		}

		inline const Vector4 & colour() const
		{
			return m_colour;
		}

		inline Vector4 & colour()
		{
			return m_colour;
		}

		inline void setScale(Vector2 scale)
		{
			m_bounds.m_width = m_textureSource.m_width * scale.x;
			m_bounds.m_height = m_textureSource.m_height * scale.y;
		}

	private:

		const Texture * m_texture;

		Rect m_bounds;

		RectI m_textureSource;

		Vector4 m_colour;
	};
}