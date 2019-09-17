#include "alvere/math/shapes/point_2d.hpp"

#include "alvere/math/shapes/circle.hpp"
#include "alvere/math/shapes/intersections.hpp"
#include "alvere/math/shapes/line_2d.hpp"
#include "alvere/math/shapes/rect.hpp"

namespace alvere
{
	bool Point2D::intersects(const Point2D& p) const
	{
		return intersection(*this, p);
	}

	bool Point2D::intersects(const Line2D& l) const
	{
		return intersection(*this, l);
	}

	bool Point2D::intersects(const Circle& c) const
	{
		return intersection(*this, c);
	}

	bool Point2D::intersects(const Rect& r) const
	{
		return intersection(*this, r);
	}

	bool Point2D::operator==(const Point2D& p) const
	{
		return position == p.position;
	}

	bool Point2D::operator!=(const Point2D& p) const
	{
		return !(*this == p);
	}
}