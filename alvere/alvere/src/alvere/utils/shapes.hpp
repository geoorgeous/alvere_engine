#pragma once

#include "alvere/math/vectors.hpp"

namespace alvere
{
	struct Rect
	{
		float m_X;
		float m_Y;
		float m_Width;
		float m_Height;

		Rect();
		Rect(float x, float y, float width, float height);

		float GetLeft() const;
		float GetRight() const;
		float GetTop() const;
		float GetBottom() const;
		alvere::Vector2 GetTopLeft() const;
		alvere::Vector2 GetBottomLeft() const;
		alvere::Vector2 GetTopRight() const;
		alvere::Vector2 GetBottomRight() const;
	};

	struct RectI
	{
		RectI();
		RectI(int x, int y, int width, int height);

		int m_X;
		int m_Y;
		int m_Width;
		int m_Height;

		int GetLeft() const;
		int GetRight() const;
		int GetTop() const;
		int GetBottom() const;
		alvere::Vector2i GetTopLeft() const;
		alvere::Vector2i GetBottomLeft() const;
		alvere::Vector2i GetTopRight() const;
		alvere::Vector2i GetBottomRight() const;
	};
}