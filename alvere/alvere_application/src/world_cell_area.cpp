#include "world_cell_area.hpp"

WorldCellArea::WorldCellArea()
	: m_Cells(nullptr), m_width(0), m_height(0)
{ }

WorldCellArea::WorldCellArea(WorldCell** cells, unsigned int width, unsigned int height)
	: m_Cells(cells), m_width(width), m_height(height)
{ }

WorldCellArea::~WorldCellArea()
{
	for (int x = 0; x < m_width; x++)
		delete[] m_Cells[x];
	delete[] m_Cells;
}

unsigned int WorldCellArea::GetWidth() const
{
	return m_width;
}

unsigned int WorldCellArea::GetHeight() const
{
	return m_height;
}

WorldCell& WorldCellArea::At(unsigned int xIdx, unsigned int yIdx)
{
	return m_Cells[xIdx][yIdx];
}

const WorldCell& WorldCellArea::At(unsigned int xIdx, unsigned int yIdx) const
{
	return m_Cells[xIdx][yIdx];
}