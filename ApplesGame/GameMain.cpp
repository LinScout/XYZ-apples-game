// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include "Constants.h"
#include "Game.h"


int main()
{
	using namespace ApplesGame;
	int seed = (int)time(nullptr);
	srand(seed);
	// Init window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Apples game!");

	// Game initialization
	Game game;
	InitGame(game);
	
	// Init game clocks
	sf::Clock gameClock;
	float lastTime = gameClock.getElapsedTime().asSeconds();

	// Main loop
	while (window.isOpen())
	{
		// Reduce framerate to not spam CPU and GPU
		sf::sleep(sf::milliseconds(16));

		// Calculate time delta
		float currentTime = gameClock.getElapsedTime().asSeconds();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// Read events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				if (!game.isInMenu && !game.showExitDialog && !game.isPaused)
				{
					game.showExitDialog = true;
					game.timeSinceLastKeyPress = 0.f;
				}
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
			{
				if (!game.isInMenu && !game.isGameFinished && !game.showExitDialog)
				{
					game.isPaused = !game.isPaused;
					if (game.isPaused)
					{
						ShowPauseMenu(game.menu);
					}
				}
			}
		}

		// Check if should close from menu
		if (ShouldCloseGame(game))
		{
			window.close();
			break;
		}

		// Update timer for key press delay
		game.timeSinceLastKeyPress += deltaTime;

		// Handle exit dialog
		if (game.showExitDialog)
		{
			if (HandleExitDialog(game))
			{
				window.close();
				break;
			}
		}
		else
		{
			UpdateGame(game, deltaTime);
		}

		// Draw game
		window.clear();
		DrawGame(game, window);

		window.display();
	}

	// Deinitialization
	DeinializeGame(game);

	return 0;
}
