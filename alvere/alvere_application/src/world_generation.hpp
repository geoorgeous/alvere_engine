#pragma once

#include "world_cell.hpp"
#include "world_cell_area.hpp"

namespace world_generation
{
	void Generate(WorldCellArea *& worldCellArea, unsigned int seed);
}