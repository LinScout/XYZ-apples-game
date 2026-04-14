#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "GameSettings.h"

namespace SnakeGame
{
	struct Snake;
	struct SnakeTextures;

	struct Food
	{
		Position2D position;
		sf::Sprite  sprite;
	};

	void InitFood(Food& food, const SnakeTextures& textures);
	void RandomizeFoodPosition(Food& food, const Snake& snake);
	void DrawFood(const Food& food, sf::RenderWindow& window);
}
