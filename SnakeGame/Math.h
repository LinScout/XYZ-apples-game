#pragma once

namespace SnakeGame
{
	struct Position2D
	{
		int x = 0;
		int y = 0;

		bool operator==(const Position2D& other) const { return x == other.x && y == other.y; }
		bool operator!=(const Position2D& other) const { return !(*this == other); }
	};

	Position2D GetRandomPositionInGrid(int gridWidth, int gridHeight);
	bool IsPositionInBounds(const Position2D& pos, int gridWidth, int gridHeight);
}
