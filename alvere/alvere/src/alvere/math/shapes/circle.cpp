#include "alvere/math/shapes/circle.hpp"

#include "alvere/math/constants.hpp"
#include "alvere/math/shapes/intersections.hpp"
#include "alvere/math/shapes/line_2d.hpp"
#include "alvere/math/shapes/point_2d.hpp"
#include "alvere/math/shapes/rect.hpp"

namespace alvere
{
	Circle Circle::unit = Circle{ Vector2::zero, 0.5f };

	float Circle::getDiameter() const
	{
		return radius * 2.0f;
	}

	float Circle::getCircumference() const
	{
		return _PI * getDiameter();
	}

	bool Circle::intersects(const Point2D& p) const
	{
		return intersection(*this, p);
	}

	bool Circle::intersects(const Line2D& l) const
	{
		return intersection(*this, l);
	}

	bool Circle::intersects(const Circle& c) const
	{
		return intersection(*this, c);
	}

	bool Circle::intersects(const Rect& r) const
	{
		return intersection(*this, r);
	}

	bool Circle::operator==(const Circle& c) const
	{
		return center == c.center && radius == c.radius;
	}

	bool Circle::operator!=(const Circle& c) const
	{
		return !(*this == c);
	}
}