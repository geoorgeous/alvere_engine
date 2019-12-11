#include <algorithm>

#include "alvere/utils/shapes.hpp"

namespace alvere
{
	Rect::Rect()
		: m_x(0), m_y(0), m_width(0), m_height(0)
	{ }

	Rect::Rect(float x, float y, float width, float height)
		: m_x(x), m_y(y), m_width(width), m_height(height)
	{ }

	float Rect::getLeft() const
	{
		return m_x;
	}

	float Rect::getRight() const
	{
		return m_x + m_width;
	}

	float Rect::getTop() const
	{
		return m_y + m_height;
	}

	float Rect::getBottom() const
	{
		return m_y;
	}

	Vector2 Rect::getTopLeft() const
	{
		return alvere::Vector2{ getLeft(), getTop() };
	}

	Vector2 Rect::getBottomLeft() const
	{
		return alvere::Vector2{ getLeft(), getBottom() };
	}

	Vector2 Rect::getTopRight() const
	{
		return alvere::Vector2{ getRight(), getTop() };
	}

	Vector2 Rect::getBottomRight() const
	{
		return alvere::Vector2{ getRight(), getBottom() };
	}

	float Rect::getArea() const
	{
		return m_width * m_height;
	}

	Rect::operator RectI() const
	{
		return RectI{ (int)m_x, (int)m_y, (int)m_width, (int)m_height };
	};

	RectI::RectI()
		: m_x(0), m_y(0), m_width(0), m_height(0)
	{ }

	RectI::RectI(int x, int y, int width, int height)
		: m_x(x), m_y(y), m_width(width), m_height(height)
	{ }

	RectI::RectI(Vector2i position, Vector2i size)
		: m_x(position[0])
		, m_y(position[1])
		, m_width(size[0])
		, m_height(size[1])
	{ }

	int RectI::getLeft() const
	{
		return m_x;
	}

	int RectI::getRight() const
	{
		return m_x + m_width;
	}

	int RectI::getTop() const
	{
		return m_y + m_height;
	}

	int RectI::getBottom() const
	{
		return m_y;
	}

	Vector2i RectI::getTopLeft() const
	{
		return alvere::Vector2i{ getLeft(), getTop() };
	}

	Vector2i RectI::getBottomLeft() const
	{
		return alvere::Vector2i{ getLeft(), getBottom() };
	}

	Vector2i RectI::getTopRight() const
	{
		return alvere::Vector2i{ getRight(), getTop() };
	}

	Vector2i RectI::getBottomRight() const
	{
		return alvere::Vector2i{ getRight(), getBottom() };
	}

	int RectI::getArea() const
	{
		return m_width * m_height;
	}

	RectI::operator Rect() const
	{
		return Rect{ (float)m_x, (float)m_y, (float)m_width, (float)m_height };
	};

	RectI RectI::overlap(RectI a, RectI b)
	{
		Vector2i min = Vector2i::max(a.getBottomLeft(), b.getBottomLeft());
		Vector2i max = Vector2i::min(a.getTopRight(), b.getTopRight());
		Vector2i size = Vector2i::max({ 0, 0 }, max - min);

		return { min, size };
	}

	RectI RectI::encapsulate(RectI a, RectI b)
	{
		alvere::Vector2i min = alvere::Vector2i::min(a.getBottomLeft(), b.getBottomLeft());
		alvere::Vector2i max = alvere::Vector2i::max(a.getTopRight(), b.getTopRight());

		return { min, max - min };
	}

	RectI RectI::encapsulate(RectI rect, alvere::Vector2i point)
	{
		alvere::Vector2i min = alvere::Vector2i::min(point, rect.getBottomLeft());
		alvere::Vector2i max = alvere::Vector2i::max(point, rect.getTopRight());

		return { min, max - min };
	}

	RectI RectI::pad(RectI rect, alvere::Vector2i amount)
	{
		Vector2i min = rect.getBottomLeft() - amount;
		Vector2i max = rect.getTopRight() + amount;
		Vector2i size = Vector2i::max({ 0, 0 }, max - min);

		return { min, size };
	}
}