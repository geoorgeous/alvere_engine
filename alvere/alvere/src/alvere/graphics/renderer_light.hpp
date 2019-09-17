#pragma once

#include <alvere/math/vector/vector_3.hpp>

#include <typeinfo>

namespace alvere
{
	struct RendererLight
	{
		Vector3 colour;

		inline bool operator==(const RendererLight & rhs) const
		{
			return colour == rhs.colour && typeid(*this) == typeid(rhs) && isEqual(rhs);
		}

	private:

		inline virtual bool isEqual(const RendererLight & other) const = 0;
	};

	struct DirectionalLight : RendererLight
	{
		Vector3 direction;

		inline bool isEqual(const RendererLight & other) const override
		{
			return direction == reinterpret_cast<const DirectionalLight &>(other).direction;
		}
	};

	struct PointLight : RendererLight
	{
		Vector3 position;
		float range;

		inline bool isEqual(const RendererLight & other) const override
		{
			const PointLight & _other = reinterpret_cast<const PointLight &>(other);
			return
				position == _other.position &&
				range == _other.range;
		}
	};

	struct SpotLight : RendererLight
	{
		Vector3 direction;
		Vector3 position;
		float range;
		float angle;

		inline bool isEqual(const RendererLight & other) const override
		{
			const SpotLight & _other = reinterpret_cast<const SpotLight &>(other);
			return
				direction == _other.direction &&
				position == _other.direction &&
				range == _other.range &&
				angle == _other.angle;
		}
	};
}