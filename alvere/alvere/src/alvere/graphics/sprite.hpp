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

		Sprite(const Texture & texture, Vector4 colour = Vector4::unit);

		Sprite(const Texture & texture, Rect bounds, Vector4 colour = Vector4::unit);

		Sprite(const Texture & texture, Rect bounds, RectI textureSource, Vector4 colour = Vector4::unit);

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

	private:

		const Texture * m_texture;

		Rect m_bounds;

		RectI m_textureSource;

		Vector4 m_colour;
	};
}