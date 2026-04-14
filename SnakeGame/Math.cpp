#include "Math.h"
#include <cstdlib>

namespace SnakeGame
{
	Position2D GetRandomPositionInGrid(int gridWidth, int gridHeight)
	{
		return { rand() % gridWidth, rand() % gridHeight };
	}

	bool IsPositionInBounds(const Position2D& pos, int gridWidth, int gridHeight)
	{
		return pos.x >= 0 && pos.x < gridWidth && pos.y >= 0 && pos.y < gridHeight;
	}
}
