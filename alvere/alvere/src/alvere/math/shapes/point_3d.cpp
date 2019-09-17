#include "alvere/math/shapes/point_3d.hpp"

#include "alvere/math/shapes/cuboid.hpp"
#include "alvere/math/shapes/intersections.hpp"
#include "alvere/math/shapes/line_3d.hpp"
#include "alvere/math/shapes/sphere.hpp"

namespace alvere
{
	bool Point3D::intersects(const Point3D& p) const
	{
		return intersection(*this, p);
	}

	bool Point3D::intersects(const Line3D& l) const
	{
		return intersection(*this, l);
	}

	bool Point3D::intersects(const Sphere& s) const
	{
		return intersection(*this, s);
	}

	bool Point3D::intersects(const Cuboid& c) const
	{
		return intersection(*this, c);
	}

	bool Point3D::operator==(const Point3D& p) const
	{
		return position == p.position;
	}

	bool Point3D::operator!=(const Point3D& p) const
	{
		return !(*this == p);
	}
}