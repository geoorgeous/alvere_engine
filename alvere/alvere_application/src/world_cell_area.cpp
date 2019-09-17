#include "world_cell_area.hpp"

WorldCellArea::WorldCellArea()
	: m_Cells(nullptr), m_Width(0), m_Height(0)
{ }

WorldCellArea::WorldCellArea(WorldCell** cells, unsigned int width, unsigned int height)
	: m_Cells(cells), m_Width(width), m_Height(height)
{ }

WorldCellArea::~WorldCellArea()
{
	for (int x = 0; x < m_Width; x++)
		delete[] m_Cells[x];
	delete[] m_Cells;
}

unsigned int WorldCellArea::GetWidth() const
{
	return m_Width;
}

unsigned int WorldCellArea::GetHeight() const
{
	return m_Height;
}

WorldCell& WorldCellArea::At(unsigned int xIdx, unsigned int yIdx)
{
	return m_Cells[xIdx][yIdx];
}

const WorldCell& WorldCellArea::At(unsigned int xIdx, unsigned int yIdx) const
{
	return m_Cells[xIdx][yIdx];
}