#include "Game.h"

namespace SnakeGame
{
	void InitGame(Game& game)
	{
		// Font
		if (!game.font.loadFromFile(FONTS_PATH + "Roboto-Bold.ttf"))
			game.font.loadFromFile("C:/Windows/Fonts/arial.ttf");

		// Textures (fallback to colored rectangles handled in draw if missing)
		LoadSnakeTextures(game.textures);

		InitSnake(game.snake, game.textures);
		InitFood(game.food, game.textures);
		InitLeaderboard(game.leaderboard, game.font);
		InitMenu(game.menu, game.font);

		game.score          = 0;
		game.isGameOver     = false;
		game.gameOverTimer  = 0.f;
		game.showLeaderboard = false;
		game.isInMenu       = true;
		game.isPaused       = false;

		game.background.setSize(sf::Vector2f((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT));
		game.background.setFillColor(sf::Color(10, 10, 10));

		game.gridLines.clear();
		for (int x = 0; x <= GRID_WIDTH; ++x)
		{
			sf::RectangleShape line({ 1.f, (float)SCREEN_HEIGHT });
			line.setPosition((float)(x * GRID_SIZE), 0.f);
			line.setFillColor(sf::Color(30, 30, 30));
			game.gridLines.push_back(line);
		}
		for (int y = 0; y <= GRID_HEIGHT; ++y)
		{
			sf::RectangleShape line({ (float)SCREEN_WIDTH, 1.f });
			line.setPosition(0.f, (float)(y * GRID_SIZE));
			line.setFillColor(sf::Color(30, 30, 30));
			game.gridLines.push_back(line);
		}

		game.scoreText.setFont(game.font);
		game.scoreText.setCharacterSize(24);
		game.scoreText.setFillColor(sf::Color::White);
		game.scoreText.setPosition(10.f, 10.f);
		game.scoreText.setString("Score: 0");

		game.gameOverText.setFont(game.font);
		game.gameOverText.setCharacterSize(60);
		game.gameOverText.setFillColor(sf::Color::Red);
		game.gameOverText.setString("GAME OVER");
		game.gameOverText.setPosition(SCREEN_WIDTH / 2.f - 180.f, SCREEN_HEIGHT / 2.f - 100.f);

		game.restartText.setFont(game.font);
		game.restartText.setCharacterSize(20);
		game.restartText.setFillColor(sf::Color::Yellow);
		game.restartText.setString("R - restart  |  M - main menu");
		game.restartText.setPosition(SCREEN_WIDTH / 2.f - 150.f, SCREEN_HEIGHT - 50.f);

		game.controlsText.setFont(game.font);
		game.controlsText.setCharacterSize(18);
		game.controlsText.setFillColor(sf::Color(200, 200, 200));
		game.controlsText.setString("Arrows: Move  |  P: Pause  |  ESC: Menu");
		game.controlsText.setPosition(SCREEN_WIDTH - 360.f, 10.f);
	}

	void StartNewGame(Game& game)
	{
		InitSnake(game.snake, game.textures);
		InitFood(game.food, game.textures);
		RandomizeFoodPosition(game.food, game.snake);

		game.score           = 0;
		game.isGameOver      = false;
		game.gameOverTimer   = 0.f;
		game.showLeaderboard = false;
		game.scoreText.setString("Score: 0");

		DifficultyLevel diff = GetDifficulty(game.menu);
		if      (diff == DifficultyLevel::Easy) game.snake.moveInterval = INITIAL_SPEED * 1.5f;
		else if (diff == DifficultyLevel::Hard) game.snake.moveInterval = INITIAL_SPEED * 0.7f;
		else                                    game.snake.moveInterval = INITIAL_SPEED;
	}

	void UpdateGame(Game& game, float deltaTime)
	{
		if (game.isInMenu)
		{
			UpdateMenu(game.menu, deltaTime);
			if (GetMenuState(game.menu) == MenuState::Playing)
			{
				game.isInMenu = false;
				StartNewGame(game);
			}
			return;
		}

		if (game.isPaused)
		{
			UpdateMenu(game.menu, deltaTime);
			if (GetMenuState(game.menu) == MenuState::Playing)
				game.isPaused = false;
			else if (GetMenuState(game.menu) == MenuState::Main)
			{
				game.isPaused = false;
				game.isInMenu = true;
				ResetMenu(game.menu);
			}
			return;
		}

		if (game.isGameOver)
		{
			game.gameOverTimer += deltaTime;
			if (game.gameOverTimer > PAUSE_LENGTH)
			{
				if (!game.showLeaderboard)
				{
					game.showLeaderboard = true;
					AddScore(game.leaderboard, "Player", game.score);
				}
				if      (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) StartNewGame(game);
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
				{
					game.isInMenu    = true;
					game.isGameOver  = false;
					game.showLeaderboard = false;
					ResetMenu(game.menu);
				}
			}
			return;
		}

		if      (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    SetSnakeDirection(game.snake, Direction::Up);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  SetSnakeDirection(game.snake, Direction::Down);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  SetSnakeDirection(game.snake, Direction::Left);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) SetSnakeDirection(game.snake, Direction::Right);

		bool shouldMove = false;
		game.snake.moveTimer += deltaTime;
		if (game.snake.moveTimer >= game.snake.moveInterval)
		{
			game.snake.moveTimer = 0.f;
			game.snake.direction = game.snake.nextDirection;
			shouldMove = true;
		}

		if (shouldMove)
			MoveSnake(game.snake, game.textures);

		const Position2D& head = GetHeadPosition(game.snake);

		if (!IsPositionInBounds(head, GRID_WIDTH, GRID_HEIGHT) || CheckSelfCollision(game.snake))
		{
			game.isGameOver    = true;
			game.gameOverTimer = 0.f;
			game.snake.isAlive = false;
			return;
		}

		if (CheckFoodCollision(game.snake, game.food.position))
		{
			GrowSnake(game.snake, game.textures);
			RandomizeFoodPosition(game.food, game.snake);
			game.score++;
			game.scoreText.setString("Score: " + std::to_string(game.score));
			
			// Check if game should finish when InfiniteApples is disabled
			if (!GetInfiniteApples(game.menu) && game.score >= MAX_APPLES)
			{
				game.isGameOver = true;
				game.gameOverTimer = 0.f;
				game.snake.isAlive = false;
				return;
			}
		}
	}

	void DrawGame(Game& game, sf::RenderWindow& window)
	{
		window.draw(game.background);

		if (game.isInMenu)
		{
			DrawMenu(game.menu, window);
			return;
		}

		for (const auto& line : game.gridLines) window.draw(line);

		DrawFood(game.food, window);
		DrawSnake(game.snake, window);

		if (!game.isGameOver)
		{
			window.draw(game.scoreText);
			window.draw(game.controlsText);
		}
		else
		{
			if (game.gameOverTimer <= PAUSE_LENGTH)
				window.draw(game.gameOverText);
			else if (game.showLeaderboard)
			{
				DrawLeaderboard(game.leaderboard, window);
				window.draw(game.restartText);
			}
		}

		if (game.isPaused)
		{
			sf::RectangleShape dim({ (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT });
			dim.setFillColor(sf::Color(0, 0, 0, 150));
			window.draw(dim);
			DrawMenu(game.menu, window);
		}
	}

	bool ShouldCloseGame(const Game& game)
	{
		return GetMenuState(game.menu) == MenuState::Exiting;
	}
}
