#include <random>

#include "world_generation.hpp"

namespace world_generation
{
	std::uniform_int_distribution<std::mt19937::result_type> udist;
	std::mt19937 rng;

	void Generate(WorldCellArea *& worldCellArea, unsigned int seed)
	{
		udist = std::uniform_int_distribution<std::mt19937::result_type>(0, 10);

		std::mt19937::result_type seedval(seed);

		rng.seed(seedval);

		unsigned int width = udist(rng);
		unsigned int height = udist(rng);

		WorldCell** cells = new WorldCell * [width];

		for (unsigned int x = 0; x < width; x++)
		{
			cells[x] = new WorldCell[height];
			for (unsigned int y = 0; y < height; y++)
			{
				if (x == 0 && y == 0) cells[x][y] = WorldCell{ (int)x, (int)y, true, WorldCell::Type::Wall_SW };
				else if (x == width - 1 && y == 0) cells[x][y] = WorldCell{ (int)x, (int)y, true, WorldCell::Type::Wall_SE };
				else if (x == 0 && y == height - 1) cells[x][y] = WorldCell{ (int)x, (int)y, true, WorldCell::Type::Wall_NW };
				else if (x == width - 1 && y == height - 1) cells[x][y] = WorldCell{ (int)x, (int)y, true, WorldCell::Type::Wall_NE };
				else if (y == 0) cells[x][y] = WorldCell{ (int)x, (int)y, true, WorldCell::Type::Wall_S };
				else if (x == width - 1) cells[x][y] = WorldCell{ (int)x, (int)y, true, WorldCell::Type::Wall_E };
				else if (y == height - 1) cells[x][y] = WorldCell{ (int)x, (int)y, true, WorldCell::Type::Wall_N };
				else if (x == 0) cells[x][y] = WorldCell{ (int)x, (int)y, true, WorldCell::Type::Wall_W };
				else cells[x][y] = WorldCell{ (int)x, (int)y, true, WorldCell::Type::Floor };
			}
		}

		worldCellArea = new WorldCellArea(cells, width, height);
	}
}