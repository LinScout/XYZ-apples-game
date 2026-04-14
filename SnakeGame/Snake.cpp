#include "Snake.h"
#include <cassert>

namespace SnakeGame
{
	// ---- helpers -------------------------------------------------------

	static sf::Vector2f CellCenter(const Position2D& pos)
	{
		return { pos.x * GRID_SIZE + GRID_SIZE / 2.f,
		         pos.y * GRID_SIZE + GRID_SIZE / 2.f };
	}

	static void FitSprite(sf::Sprite& sprite, const sf::Texture& tex)
	{
		sprite.setTexture(tex, true);
		sf::FloatRect b = sprite.getLocalBounds();
		sprite.setOrigin(b.width / 2.f, b.height / 2.f);
		sprite.setScale((float)GRID_SIZE / b.width, (float)GRID_SIZE / b.height);
	}

	static void FitBodySprite(sf::Sprite& sprite, const sf::Texture& tex)
	{
		sprite.setTexture(tex, true);
		sf::FloatRect b = sprite.getLocalBounds();
		sprite.setOrigin(b.width / 2.f, b.height / 2.f);
		// For body segments, make them slightly smaller to fit perfectly in grid
		float scale = (float)GRID_SIZE / b.width * 0.98f; // 2% smaller to eliminate gaps
		sprite.setScale(scale, scale);
	}

	// ---- texture selection ---------------------------------------------

	bool LoadSnakeTextures(SnakeTextures& t)
	{
		bool ok = true;
		ok &= t.head_up.loadFromFile       (TEXTURES_PATH + "head_up.png");
		ok &= t.head_down.loadFromFile     (TEXTURES_PATH + "head_down.png");
		ok &= t.head_left.loadFromFile     (TEXTURES_PATH + "head_left.png");
		ok &= t.head_right.loadFromFile    (TEXTURES_PATH + "head_right.png");
		ok &= t.body_vertical.loadFromFile  (TEXTURES_PATH + "body_vertical.png");
		ok &= t.body_horizontal.loadFromFile(TEXTURES_PATH + "body_horizontal.png");
		ok &= t.body_topleft.loadFromFile   (TEXTURES_PATH + "body_topleft.png");
		ok &= t.body_topright.loadFromFile  (TEXTURES_PATH + "body_topright.png");
		ok &= t.body_bottomleft.loadFromFile (TEXTURES_PATH + "body_bottomleft.png");
		ok &= t.body_bottomright.loadFromFile(TEXTURES_PATH + "body_bottomright.png");
		ok &= t.tail_up.loadFromFile       (TEXTURES_PATH + "tail_up.png");
		ok &= t.tail_down.loadFromFile     (TEXTURES_PATH + "tail_down.png");
		ok &= t.tail_left.loadFromFile     (TEXTURES_PATH + "tail_left.png");
		ok &= t.tail_right.loadFromFile    (TEXTURES_PATH + "tail_right.png");
		ok &= t.apple.loadFromFile         (TEXTURES_PATH + "apple.png");
		return ok;
	}

	const sf::Texture& GetHeadTexture(const SnakeTextures& t, Direction dir)
	{
		switch (dir)
		{
		case Direction::Up:    return t.head_up;
		case Direction::Down:  return t.head_down;
		case Direction::Left:  return t.head_left;
		default:               return t.head_right;
		}
	}

	const sf::Texture& GetTailTexture(const SnakeTextures& t, Direction dir)
	{
		switch (dir)
		{
		case Direction::Up:    return t.tail_up;
		case Direction::Down:  return t.tail_down;
		case Direction::Left:  return t.tail_left;
		default:               return t.tail_right;
		}
	}

	// from = direction the segment came from (where prev segment is)
	// to   = direction the segment goes to   (where next segment is)
	// For a straight segment from==to; for a bend they differ.
	const sf::Texture& GetBodyTexture(const SnakeTextures& t, Direction from, Direction to)
	{
		// Straight
		if (from == to)
		{
			if (from == Direction::Up || from == Direction::Down) return t.body_vertical;
			return t.body_horizontal;
		}
		// Bends — order-independent pairs
		auto is = [&](Direction a, Direction b) {
			return (from == a && to == b) || (from == b && to == a);
		};
		if (is(Direction::Up,   Direction::Right)) return t.body_topright;
		if (is(Direction::Up,   Direction::Left))  return t.body_topleft;
		if (is(Direction::Down, Direction::Right)) return t.body_bottomright;
		if (is(Direction::Down, Direction::Left))  return t.body_bottomleft;
		// fallback
		return t.body_vertical;
	}

	// ---- direction of the tail segment (points away from body) ---------
	// The tail points in the direction AWAY from the segment before it.
	static Direction TailDirection(const Snake& snake)
	{
		if (snake.segments.size() < 2) return snake.direction;
		const Position2D& tail = snake.segments.back().position;
		const Position2D& prev = snake.segments[snake.segments.size() - 2].position;
		int dx = tail.x - prev.x;
		int dy = tail.y - prev.y;
		if (dx ==  1) return Direction::Right;
		if (dx == -1) return Direction::Left;
		if (dy ==  1) return Direction::Down;
		return Direction::Up;
	}

	// ---- rebuild all sprites after a move ------------------------------
	static void RefreshSprites(Snake& snake, const SnakeTextures& tex)
	{
		size_t n = snake.segments.size();
		for (size_t i = 0; i < n; ++i)
		{
			SnakeSegment& seg = snake.segments[i];
			const sf::Texture* t = nullptr;

			if (i == 0) // head
			{
				t = &GetHeadTexture(tex, snake.direction);
			}
			else if (i == n - 1) // tail
			{
				t = &GetTailTexture(tex, TailDirection(snake));
			}
			else // body
			{
				// from = direction toward previous segment
				const Position2D& prev = snake.segments[i - 1].position;
				const Position2D& cur  = snake.segments[i].position;
				const Position2D& next = snake.segments[i + 1].position;

				// direction from cur toward prev
				Direction toPrev, toNext;
				int dx = prev.x - cur.x, dy = prev.y - cur.y;
				if      (dx ==  1) toPrev = Direction::Right;
				else if (dx == -1) toPrev = Direction::Left;
				else if (dy ==  1) toPrev = Direction::Down;
				else               toPrev = Direction::Up;

				dx = next.x - cur.x; dy = next.y - cur.y;
				if      (dx ==  1) toNext = Direction::Right;
				else if (dx == -1) toNext = Direction::Left;
				else if (dy ==  1) toNext = Direction::Down;
				else               toNext = Direction::Up;

				t = &GetBodyTexture(tex, toPrev, toNext);
			}

			// Use appropriate fitting function based on segment type
			if (i == 0 || i == n - 1) // head or tail
				FitSprite(seg.sprite, *t);
			else // body
				FitBodySprite(seg.sprite, *t);
			seg.sprite.setPosition(CellCenter(seg.position));
		}
	}

	// ---- public API ----------------------------------------------------

	void InitSnake(Snake& snake, const SnakeTextures& tex)
	{
		snake.segments.clear();
		snake.direction     = Direction::Right;
		snake.nextDirection = Direction::Right;
		snake.moveTimer     = 0.f;
		snake.moveInterval  = INITIAL_SPEED;
		snake.isAlive       = true;
		snake.shouldGrowTail = false;

		int startX = GRID_WIDTH  / 2;
		int startY = GRID_HEIGHT / 2;

		for (int i = 0; i < INITIAL_SNAKE_LENGTH; ++i)
		{
			SnakeSegment seg;
			seg.position = { startX - i, startY };
			snake.segments.push_back(seg);
		}

		RefreshSprites(snake, tex);
	}

	void SetSnakeDirection(Snake& snake, Direction dir)
	{
		if ((snake.direction == Direction::Up    && dir == Direction::Down)  ||
			(snake.direction == Direction::Down  && dir == Direction::Up)    ||
			(snake.direction == Direction::Left  && dir == Direction::Right) ||
			(snake.direction == Direction::Right && dir == Direction::Left))
			return;
		snake.nextDirection = dir;
	}

	void UpdateSnake(Snake& snake, float deltaTime)
	{
		if (!snake.isAlive) return;
		snake.moveTimer += deltaTime;
		if (snake.moveTimer >= snake.moveInterval)
		{
			snake.moveTimer = 0.f;
			snake.direction = snake.nextDirection;
			// MoveSnake is called from Game with textures
		}
	}

	void MoveSnake(Snake& snake, const SnakeTextures& tex)
	{
		if (snake.segments.empty()) return;

		Position2D newHead = snake.segments[0].position;
		switch (snake.direction)
		{
		case Direction::Up:    newHead.y--; break;
		case Direction::Down:  newHead.y++; break;
		case Direction::Left:  newHead.x--; break;
		case Direction::Right: newHead.x++; break;
		}

		// Move segments - if should grow, don't move the last segment (tail)
		if (snake.shouldGrowTail && snake.segments.size() > 1)
		{
			// Move all segments except the last one (tail)
			for (size_t i = snake.segments.size() - 1; i > 0; --i)
				snake.segments[i].position = snake.segments[i - 1].position;
		}
		else
		{
			// Move all segments normally
			for (size_t i = snake.segments.size() - 1; i > 0; --i)
				snake.segments[i].position = snake.segments[i - 1].position;
		}

		snake.segments[0].position = newHead;
		
		// Reset grow flag
		snake.shouldGrowTail = false;

		RefreshSprites(snake, tex);
	}

	void GrowSnake(Snake& snake, const SnakeTextures& tex)
	{
		if (snake.segments.empty()) return;

		// Add new segment at tail position
		SnakeSegment seg;
		seg.position = snake.segments.back().position;
		snake.segments.push_back(seg);

		// Set flag to prevent tail movement on next move
		snake.shouldGrowTail = true;

		snake.moveInterval -= SPEED_INCREASE;
		if (snake.moveInterval < MIN_SPEED) snake.moveInterval = MIN_SPEED;

		RefreshSprites(snake, tex);
	}

	bool CheckSelfCollision(const Snake& snake)
	{
		if (snake.segments.size() < 2) return false;
		const Position2D& head = snake.segments[0].position;
		for (size_t i = 1; i < snake.segments.size(); ++i)
			if (head == snake.segments[i].position) return true;
		return false;
	}

	bool CheckFoodCollision(const Snake& snake, const Position2D& foodPos)
	{
		if (snake.segments.empty()) return false;
		return snake.segments[0].position == foodPos;
	}

	void DrawSnake(const Snake& snake, sf::RenderWindow& window)
	{
		for (int i = (int)snake.segments.size() - 1; i >= 0; --i)
			window.draw(snake.segments[i].sprite);
	}

	const Position2D& GetHeadPosition(const Snake& snake)
	{
		return snake.segments[0].position;
	}
}
