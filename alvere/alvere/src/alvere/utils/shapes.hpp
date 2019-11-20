#pragma once

#include "alvere/math/vectors.hpp"

namespace alvere
{
	struct RectI;

	struct Rect
	{
		float m_x;

		float m_y;

		float m_width;

		float m_height;

		Rect();

		Rect(float x, float y, float width, float height);

		float getLeft() const;

		float getRight() const;

		float getTop() const;

		float getBottom() const;

		alvere::Vector2 getTopLeft() const;

		alvere::Vector2 getBottomLeft() const;

		alvere::Vector2 getTopRight() const;

		alvere::Vector2 getBottomRight() const;

		float getArea() const;

		operator explicit RectI() const;
	};

	struct RectI
	{
		RectI();

		RectI(int x, int y, int width, int height);

		int m_x;

		int m_y;

		int m_width;

		int m_height;

		int getLeft() const;

		int getRight() const;

		int getTop() const;

		int getBottom() const;

		alvere::Vector2i getTopLeft() const;

		alvere::Vector2i getBottomLeft() const;

		alvere::Vector2i getTopRight() const;

		alvere::Vector2i getBottomRight() const;

		int getArea() const;

		inline bool intersects(const RectI other) const
		{
			return !(other.getLeft() > getRight() ||
				other.getRight() < getLeft() ||
				other.getTop() < getBottom() ||
				other.getBottom() > getTop());
		}

		operator explicit Rect() const;
	};
}