#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Math.h"
#include "GameSettings.h"

namespace SnakeGame
{
	enum class Direction { Up, Down, Left, Right };

	struct SnakeTextures
	{
		// Head — one per direction
		sf::Texture head_up, head_down, head_left, head_right;
		// Body straight
		sf::Texture body_vertical, body_horizontal;
		// Body bends
		sf::Texture body_topleft, body_topright, body_bottomleft, body_bottomright;
		// Tail — one per direction
		sf::Texture tail_up, tail_down, tail_left, tail_right;
		// Food
		sf::Texture apple;
	};

	struct SnakeSegment
	{
		Position2D position;
		Direction  from = Direction::Right; // direction we came FROM (prev segment)
		Direction  to   = Direction::Right; // direction we go TO   (next segment)
		sf::Sprite sprite;
	};

	struct Snake
	{
		std::vector<SnakeSegment> segments;
		Direction direction     = Direction::Right;
		Direction nextDirection = Direction::Right;
		float moveTimer         = 0.f;
		float moveInterval      = INITIAL_SPEED;
		bool  isAlive           = true;
		bool  shouldGrowTail    = false;
	};

	bool LoadSnakeTextures(SnakeTextures& tex);

	void InitSnake(Snake& snake, const SnakeTextures& tex);
	void SetSnakeDirection(Snake& snake, Direction dir);
	void UpdateSnake(Snake& snake, float deltaTime);
	void MoveSnake(Snake& snake, const SnakeTextures& tex);
	void GrowSnake(Snake& snake, const SnakeTextures& tex);
	bool CheckSelfCollision(const Snake& snake);
	bool CheckFoodCollision(const Snake& snake, const Position2D& foodPos);
	void DrawSnake(const Snake& snake, sf::RenderWindow& window);
	const Position2D& GetHeadPosition(const Snake& snake);

	// Returns the correct texture for a body/head/tail segment
	const sf::Texture& GetHeadTexture(const SnakeTextures& tex, Direction dir);
	const sf::Texture& GetTailTexture(const SnakeTextures& tex, Direction dir);
	const sf::Texture& GetBodyTexture(const SnakeTextures& tex, Direction from, Direction to);
}
