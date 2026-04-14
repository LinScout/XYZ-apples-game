#include <SFML/Graphics.hpp>
#include <ctime>
#include "Game.h"

using namespace SnakeGame;

int main()
{
	srand((unsigned int)time(nullptr));

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Snake Game");

	Game* game = new Game();
	InitGame(*game);

	sf::Clock clock;
	float lastTime = clock.getElapsedTime().asSeconds();

	while (window.isOpen())
	{
		float currentTime = clock.getElapsedTime().asSeconds();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape && !game->isInMenu && !game->isPaused)
				{
					game->isInMenu = true;
					ResetMenu(game->menu);
				}
				else if (event.key.code == sf::Keyboard::P && !game->isInMenu && !game->isGameOver)
				{
					game->isPaused = !game->isPaused;
					if (game->isPaused) ShowPauseMenu(game->menu);
				}
			}
		}

		if (ShouldCloseGame(*game)) { window.close(); break; }

		UpdateGame(*game, deltaTime);

		window.clear();
		DrawGame(*game, window);
		window.display();

		float elapsed = clock.getElapsedTime().asSeconds() - currentTime;
		if (elapsed < TIME_PER_FRAME)
			sf::sleep(sf::seconds(TIME_PER_FRAME - elapsed));
	}

	delete game;
	return 0;
}
