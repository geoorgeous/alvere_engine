#pragma once

class WorldCell
{
public:
	enum class Type
	{
		Void,

		Wall_N,
		Wall_E,
		Wall_S,
		Wall_W,

		Wall_NW,
		Wall_NE,
		Wall_SW,
		Wall_SE,

		Floor
	};

	int m_X;
	int m_Y;
	bool m_IsWalkable;
	Type m_Type;
};