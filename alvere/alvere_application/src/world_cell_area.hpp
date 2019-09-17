#pragma once

#include <vector>

#include "world_cell.hpp"

class WorldCellArea
{
public:
	WorldCellArea();
	WorldCellArea(WorldCell ** cells, unsigned int width, unsigned int height);
	~WorldCellArea();

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	WorldCell& At(unsigned int xIdx, unsigned int yIdx);
	const WorldCell& At(unsigned int xIdx, unsigned int yIdx) const;

private:
	WorldCell ** m_Cells;
	unsigned int m_Width;
	unsigned int m_Height;
};