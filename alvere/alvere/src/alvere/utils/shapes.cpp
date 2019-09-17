#include "alvere/utils/shapes.hpp"

namespace alvere
{
	Rect::Rect()
		: m_X(0), m_Y(0), m_Width(0), m_Height(0)
	{ }

	Rect::Rect(float x, float y, float width, float height)
		: m_X(x), m_Y(y), m_Width(width), m_Height(height)
	{ }

	float Rect::GetLeft() const
	{
		return m_X;
	}

	float Rect::GetRight() const
	{
		return m_X + m_Width;
	}

	float Rect::GetTop() const
	{
		return m_Y + m_Height;
	}

	float Rect::GetBottom() const
	{
		return m_Y;
	}

	alvere::Vector2 Rect::GetTopLeft() const
	{
		return alvere::Vector2{ GetLeft(), GetTop() };
	}

	alvere::Vector2 Rect::GetBottomLeft() const
	{
		return alvere::Vector2{ GetLeft(), GetBottom() };
	}

	alvere::Vector2 Rect::GetTopRight() const
	{
		return alvere::Vector2{ GetRight(), GetTop() };
	}

	alvere::Vector2 Rect::GetBottomRight() const
	{
		return alvere::Vector2{ GetRight(), GetBottom() };
	}

	RectI::RectI()
		: m_X(0), m_Y(0), m_Width(0), m_Height(0)
	{ }

	RectI::RectI(int x, int y, int width, int height)
		: m_X(x), m_Y(y), m_Width(width), m_Height(height)
	{ }

	int RectI::GetLeft() const
	{
		return m_X;
	}

	int RectI::GetRight() const
	{
		return m_X + m_Width;
	}

	int RectI::GetTop() const
	{
		return m_Y + m_Height;
	}

	int RectI::GetBottom() const
	{
		return m_Y;
	}

	alvere::Vector2i RectI::GetTopLeft() const
	{
		return alvere::Vector2i{ GetLeft(), GetTop() };
	}

	alvere::Vector2i RectI::GetBottomLeft() const
	{
		return alvere::Vector2i{ GetLeft(), GetBottom() };
	}

	alvere::Vector2i RectI::GetTopRight() const
	{
		return alvere::Vector2i{ GetRight(), GetTop() };
	}

	alvere::Vector2i RectI::GetBottomRight() const
	{
		return alvere::Vector2i{ GetRight(), GetBottom() };
	}
}