#pragma once

namespace alvere
{
	template<typename T>
	struct Vec2_
	{
		union
		{
			T x;
			T r;
			T u = 0;
		};

		union
		{
			T y;
			T g;
			T v = 0;
		};

		Vec2_(T v = 0)
			: x(v), y(v)
		{ }

		Vec2_(T v1, T v2)
			: x(v1), y(v2)
		{ }

		Vec2_<T> & operator=(const Vec2_<T> & rhs)
		{
			x = rhs.x;
			y = rhs.y;
			return *this;
		}

		inline Vec2_<T> & operator+=(const Vec2_<T> & rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}
	};

	template <typename T>
	inline bool operator==(const Vec2_<T> & lhs, const Vec2_<T> & rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	template <typename T>
	inline bool operator!=(const Vec2_<T> & lhs, const Vec2_<T> & rhs)
	{
		return !operator==(lhs, rhs);
	}

	template <typename T>
	inline Vec2_<T> operator+(Vec2_<T> lhs, const Vec2_<T> & rhs)
	{
		lhs += rhs;
		return lhs;
	}
}