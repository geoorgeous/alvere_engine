#include "alvere/math/shapes/line_2d.hpp"

#include "alvere/math/shapes/circle.hpp"
#include "alvere/math/shapes/intersections.hpp"
#include "alvere/math/shapes/point_2d.hpp"
#include "alvere/math/shapes/rect.hpp"

namespace alvere
{
	float Line2D::getLength() const
	{
		return p1.distance(p2);
	}

	bool Line2D::intersects(const Point2D& p) const
	{
		return intersection(*this, p);
	}

	bool Line2D::intersects(const Line2D& l) const
	{
		return intersection(*this, l);
	}

	bool Line2D::intersects(const Circle& c) const
	{
		return intersection(*this, c);
	}

	bool Line2D::intersects(const Rect& r) const
	{
		return intersection(*this, r);
	}

	bool Line2D::operator==(const Line2D& l) const
	{
		return p1 == l.p1 && p2 == l.p2 ||
			p1 == l.p2 && p2 == l.p1;
	}

	bool Line2D::operator!=(const Line2D& l) const
	{
		return !(*this == l);
	}
}