#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "GameSettings.h"
#include "Snake.h"
#include "Food.h"
#include "Leaderboard.h"
#include "Menu.h"

namespace SnakeGame
{
	struct Game
	{
		SnakeTextures textures;   // loaded once, shared by snake & food

		Snake       snake;
		Food        food;
		Leaderboard leaderboard;
		Menu        menu;

		int   score         = 0;
		bool  isGameOver    = false;
		float gameOverTimer = 0.f;
		bool  showLeaderboard = false;
		bool  isInMenu      = true;
		bool  isPaused      = false;

		sf::Font font;

		sf::Text           scoreText;
		sf::Text           gameOverText;
		sf::Text           restartText;
		sf::Text           controlsText;
		sf::RectangleShape background;
		std::vector<sf::RectangleShape> gridLines;
	};

	void InitGame(Game& game);
	void UpdateGame(Game& game, float deltaTime);
	void DrawGame(Game& game, sf::RenderWindow& window);
	void StartNewGame(Game& game);
	bool ShouldCloseGame(const Game& game);
}
