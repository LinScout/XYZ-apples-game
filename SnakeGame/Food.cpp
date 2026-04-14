#include "Food.h"
#include "Snake.h"

namespace SnakeGame
{
	static sf::Vector2f CellCenter(const Position2D& pos)
	{
		return {
			pos.x * GRID_SIZE + GRID_SIZE / 2.f,
			pos.y * GRID_SIZE + GRID_SIZE / 2.f
		};
	}

	void InitFood(Food& food, const SnakeTextures& textures)
	{
		food.sprite.setTexture(textures.apple, true);
		sf::FloatRect b = food.sprite.getLocalBounds();
		food.sprite.setOrigin(b.width / 2.f, b.height / 2.f);
		food.sprite.setScale(
			(float)GRID_SIZE / b.width,
			(float)GRID_SIZE / b.height);
		food.position = GetRandomPositionInGrid(GRID_WIDTH, GRID_HEIGHT);
		food.sprite.setPosition(CellCenter(food.position));
	}

	void RandomizeFoodPosition(Food& food, const Snake& snake)
	{
		Position2D newPos;
		bool valid = false;
		int attempts = 0;
		while (!valid && attempts++ < 100)
		{
			newPos = GetRandomPositionInGrid(GRID_WIDTH, GRID_HEIGHT);
			valid = true;
			for (const auto& seg : snake.segments)
				if (newPos == seg.position) { valid = false; break; }
		}
		food.position = newPos;
		food.sprite.setPosition(CellCenter(newPos));
	}

	void DrawFood(const Food& food, sf::RenderWindow& window)
	{
		window.draw(food.sprite);
	}
}
