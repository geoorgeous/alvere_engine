#include "alvere/math/shapes/rect.hpp"

#include "alvere/math/shapes/circle.hpp"
#include "alvere/math/shapes/intersections.hpp"
#include "alvere/math/shapes/line_2d.hpp"
#include "alvere/math/shapes/point_2d.hpp"

namespace alvere
{
	Rect Rect::unit = Rect{ Vector2::zero, Vector2(0.5f) };

	float Rect::getWidth() const
	{
		return halfWidth * 2.0f;
	}

	float Rect::getHeight() const
	{
		return halfHeight * 2.0f;
	}

	float Rect::getLeft() const
	{
		return center.x - halfWidth;
	}

	float Rect::getRight() const
	{
		return center.x + halfWidth;
	}

	float Rect::getTop() const
	{
		return center.y + halfHeight;
	}

	float Rect::getBottom() const
	{
		return center.y - halfHeight;
	}

	Vector2 Rect::getTopLeft() const
	{
		return Vector2{ getLeft(), getTop() };
	}

	Vector2 Rect::getTopRight() const
	{
		return Vector2{ getRight(), getTop() };
	}

	Vector2 Rect::getBottomLeft() const
	{
		return Vector2{ getLeft(), getBottom() };
	}

	Vector2 Rect::getBottomRight() const
	{
		return Vector2{ getRight(), getBottom() };
	}

	float Rect::getPerimeter() const
	{
		return halfWidth * 4.0f + halfHeight * 4.0f;
	}

	bool Rect::intersects(const Point2D& p) const
	{
		return intersection(*this, p);
	}

	bool Rect::intersects(const Line2D& l) const
	{
		return intersection(*this, l);
	}

	bool Rect::intersects(const Circle& c) const
	{
		return intersection(*this, c);
	}

	bool Rect::intersects(const Rect& r) const
	{
		return intersection(*this, r);
	}

	Rect& Rect::operator=(const Rect& r)
	{
		center = r.center;
		halfExtents = r.halfExtents;
		return *this;
	}

	bool Rect::operator==(const Rect& r) const
	{
		return center == r.center && halfExtents == r.halfExtents;
	}

	bool Rect::operator!=(const Rect& r) const
	{
		return !(*this == r);
	}
}