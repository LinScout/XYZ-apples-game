#include "Game.h"
#include <cassert>

using namespace std;

namespace ApplesGame
{
	bool HandleExitDialog(Game& game)
	{
		// Возвращает true, если нужно закрыть игру
		// Добавляем задержку для предотвращения мгновенного срабатывания
		const float KEY_PRESS_DELAY = 0.2f;
		
		if (game.timeSinceLastKeyPress < KEY_PRESS_DELAY)
		{
			return false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
		{
			return true; // Закрыть игру
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
		{
			game.showExitDialog = false; // Закрыть диалог
			game.timeSinceLastKeyPress = 0.f;
		}
		return false;
	}

	void StartPlayingState(Game& game)
	{
		SetPlayerPosition(game.player, { SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f });
		SetPlayerSpeed(game.player, INITIAL_SPEED);
		SetPlayerDirection(game.player, PlayerDirection::Right);

		// Init apples - сначала проверяем размер, если нужно - создаем
		if (game.apples.size() != NUM_APPLES)
		{
			game.apples.clear();
			game.apples.resize(NUM_APPLES);
			for (int i = 0; i < NUM_APPLES; ++i)
			{
				InitApple(game.apples[i], game);
			}
		}
		
		// Устанавливаем позиции яблок
		for (int i = 0; i < NUM_APPLES; ++i)
		{
			SetApplePosition(game.apples[i], GetRandomPositionInRectangle(game.screenRect));
		}

		// Init rocks - сначала проверяем размер, если нужно - создаем
		if (game.rocks.size() != NUM_ROCKS)
		{
			game.rocks.clear();
			game.rocks.resize(NUM_ROCKS);
			for (int i = 0; i < NUM_ROCKS; ++i)
			{
				InitRock(game.rocks[i], game);
			}
		}
		
		// Устанавливаем позиции камней
		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			SetRockPosition(game.rocks[i], GetRandomPositionInRectangle(game.screenRect));
		}

		game.numEatenApples = 0;
		game.isGameFinished = false;
		game.timeSinceGameFinish = 0;
		game.timeSinceLastKeyPress = 0.f;
		game.scoreText.setString("Apples eaten: " + to_string(game.numEatenApples));
	}

	void UpdatePlayingState(Game& game, float deltaTime)
	{
		// Handle input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			SetPlayerDirection(game.player, PlayerDirection::Right);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			SetPlayerDirection(game.player, PlayerDirection::Up);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			SetPlayerDirection(game.player, PlayerDirection::Left);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			SetPlayerDirection(game.player, PlayerDirection::Down);
		}

		UpdatePlayer(game.player, deltaTime);

		// Find player collisions with apples
		for (size_t i = 0; i < game.apples.size(); ++i)
		{
			if (DoShapesCollide(GetPlayerCollider(game.player), GetAppleCollider(game.apples[i])))
			{
				SetApplePosition(game.apples[i], GetRandomPositionInRectangle(game.screenRect));
				++game.numEatenApples;
				SetPlayerSpeed(game.player, GetPlayerSpeed(game.player) + ACCELERATION);
				game.eatAppleSound.play();
				game.scoreText.setString("Apples eaten: " + to_string(game.numEatenApples));
			}
		}

		// Find player collisions with rocks
		for (size_t i = 0; i < game.rocks.size(); ++i)
		{
			if (DoShapesCollide(GetPlayerCollider(game.player), GetRockCollider(game.rocks[i])))
			{
				StartGameoverState(game);
			}
		}

		// Check screen borders collision
		if (!DoShapesCollide(GetPlayerCollider(game.player), game.screenRect))
		{
			StartGameoverState(game);
		}
	}

	void StartGameoverState(Game& game)
	{
		game.isGameFinished = true;
		game.timeSinceGameFinish = 0.f;
		game.showLeaderboard = false;
		game.gameOverSound.play();
		game.gameOverScoreText.setString("Your scores: " + to_string(game.numEatenApples));
	}

	void UpdateGameoverState(Game& game, float deltaTime)
	{
		if (game.timeSinceGameFinish <= PAUSE_LENGTH)
		{
			game.timeSinceGameFinish += deltaTime;
			game.background.setFillColor(sf::Color::Red);
		}
		else
		{
			// Показываем таблицу лидеров
			if (!game.showLeaderboard)
			{
				game.showLeaderboard = true;
				game.background.setFillColor(sf::Color::Black);
				
				// Добавляем игрока в таблицу лидеров
				if (game.leaderboard.currentType == LeaderboardType::Map)
				{
					AddPlayerToLeaderboardMap(game.leaderboard, "Player", game.numEatenApples);
				}
				else if (game.leaderboard.currentType == LeaderboardType::UnorderedMap)
				{
					AddPlayerToLeaderboardUnorderedMap(game.leaderboard, "Player", game.numEatenApples);
				}
				else // Vector
				{
					AddPlayerToLeaderboard(game.leaderboard, "Player", game.numEatenApples);
				}
			}

			// Ждем нажатия R для рестарта
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				// При рестарте обновляем только очки игрока
				if (game.leaderboard.currentType == LeaderboardType::Map)
				{
					// Для map версии просто обновляем значение
					game.leaderboard.recordsMap.erase("Player");
				}
				else if (game.leaderboard.currentType == LeaderboardType::UnorderedMap)
				{
					// Для unordered_map версии просто обновляем значение
					game.leaderboard.recordsUnorderedMap.erase("Player");
				}
				else // Vector
				{
					// Для vector версии удаляем старую запись игрока
					for (size_t i = 0; i < game.leaderboard.records.size(); ++i)
					{
						if (game.leaderboard.records[i].name == "Player")
						{
							game.leaderboard.records.erase(game.leaderboard.records.begin() + i);
							break;
						}
					}
				}

				StartPlayingState(game);
			}
			
			// Нажатие M для возврата в меню
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
			{
				game.isInMenu = true;
				game.isGameFinished = false;
				game.showLeaderboard = false;
				ResetMenu(game.menu);
			}
		}
	}

	void InitGame(Game& game)
	{
		// Load resources
		assert(game.playerTexture.loadFromFile(RESOURCES_PATH + "\\Player.png"));
		assert(game.appleTexture.loadFromFile(RESOURCES_PATH + "\\Apple.png"));
		assert(game.rockTexture.loadFromFile(RESOURCES_PATH + "\\Rock.png"));
		assert(game.eatAppleSoundBuffer.loadFromFile(RESOURCES_PATH + "\\AppleEat.wav"));
		assert(game.gameOverSoundBuffer.loadFromFile(RESOURCES_PATH + "\\Death.wav"));
		assert(game.font.loadFromFile(RESOURCES_PATH + "\\Fonts\\Roboto-Bold.ttf"));

		// Init game objects
		game.screenRect = { 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT };

		InitPlayer(game.player, game);

		// Init apples
		game.apples.resize(NUM_APPLES);
		for (int i = 0; i < NUM_APPLES; ++i)
		{
			InitApple(game.apples[i], game);
		}

		// Init rocks
		game.rocks.resize(NUM_ROCKS);
		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			InitRock(game.rocks[i], game);
		}

		// Init background
		game.background.setSize(sf::Vector2f(game.screenRect.size.x, game.screenRect.size.y));
		game.background.setFillColor(sf::Color::Black);
		game.background.setPosition(0.f, 0.f);

		// Init sounds
		game.eatAppleSound.setBuffer(game.eatAppleSoundBuffer);
		game.gameOverSound.setBuffer(game.gameOverSoundBuffer);

		// Init texts
		game.scoreText.setFont(game.font);
		game.scoreText.setCharacterSize(20);
		game.scoreText.setFillColor(sf::Color::White);
		game.scoreText.setPosition(20.f, 10.f);

		game.controlsHintText.setFont(game.font);
		game.controlsHintText.setCharacterSize(20);
		game.controlsHintText.setFillColor(sf::Color::White);
		game.controlsHintText.setString("Use arrows to move, ESC to exit");
		game.controlsHintText.setPosition(SCREEN_WIDTH - game.controlsHintText.getGlobalBounds().width - 20.f, 10.f);

		game.gameOverText.setFont(game.font);
		game.gameOverText.setCharacterSize(100);
		game.gameOverText.setFillColor(sf::Color::White);
		game.gameOverText.setPosition(SCREEN_WIDTH / 2.f - 200.f, SCREEN_HEIGHT / 2.f - 50.f);
		game.gameOverText.setString("Game Over");

		game.gameOverScoreText.setFont(game.font);
		game.gameOverScoreText.setCharacterSize(30);
		game.gameOverScoreText.setFillColor(sf::Color::White);
		game.gameOverScoreText.setString("Your score: " + to_string(game.numEatenApples));
		game.gameOverScoreText.setPosition(SCREEN_WIDTH / 2.f - game.controlsHintText.getGlobalBounds().width / 4.f , SCREEN_HEIGHT / 2.f + 50.f);

		game.restartHintText.setFont(game.font);
		game.restartHintText.setCharacterSize(20);
		game.restartHintText.setFillColor(sf::Color::Yellow);
		game.restartHintText.setString("Press R to restart | Press M for menu");
		game.restartHintText.setPosition(SCREEN_WIDTH / 2.f - 180.f, SCREEN_HEIGHT - 50.f);

		// Инициализация диалога выхода
		game.exitDialogBackground.setSize(sf::Vector2f(400.f, 200.f));
		game.exitDialogBackground.setFillColor(sf::Color(0, 0, 0, 200));
		game.exitDialogBackground.setPosition(SCREEN_WIDTH / 2.f - 200.f, SCREEN_HEIGHT / 2.f - 100.f);
		game.exitDialogBackground.setOutlineColor(sf::Color::White);
		game.exitDialogBackground.setOutlineThickness(3.f);

		game.exitDialogText.setFont(game.font);
		game.exitDialogText.setCharacterSize(30);
		game.exitDialogText.setFillColor(sf::Color::White);
		game.exitDialogText.setString("Do you want to exit?");
		game.exitDialogText.setPosition(SCREEN_WIDTH / 2.f - 150.f, SCREEN_HEIGHT / 2.f - 70.f);

		game.exitDialogYesText.setFont(game.font);
		game.exitDialogYesText.setCharacterSize(24);
		game.exitDialogYesText.setFillColor(sf::Color::Green);
		game.exitDialogYesText.setString("Yes (Y)");
		game.exitDialogYesText.setPosition(SCREEN_WIDTH / 2.f - 120.f, SCREEN_HEIGHT / 2.f + 20.f);

		game.exitDialogNoText.setFont(game.font);
		game.exitDialogNoText.setCharacterSize(24);
		game.exitDialogNoText.setFillColor(sf::Color::Red);
		game.exitDialogNoText.setString("No (N)");
		game.exitDialogNoText.setPosition(SCREEN_WIDTH / 2.f + 30.f, SCREEN_HEIGHT / 2.f + 20.f);

		// Инициализация таблицы лидеров
		InitLeaderboard(game.leaderboard, game.font, LeaderboardType::Vector);
		
		// Инициализация меню
		InitMenu(game.menu, game.font);
		game.isInMenu = true;

		StartPlayingState(game);
	}

	bool ShouldCloseGame(const Game& game)
	{
		return GetMenuState(game.menu) == MenuState::Exiting;
	}

	void UpdateGame(Game& game, float deltaTime)
	{
		// Update menu or game
		if (game.isInMenu)
		{
			UpdateMenu(game.menu, deltaTime);
			
			// Check if player selected Play
			if (GetMenuState(game.menu) == MenuState::Playing)
			{
				game.isInMenu = false;
				
				// Apply leaderboard type settings
				LeaderboardTypeOption lbType = GetLeaderboardType(game.menu);
				LeaderboardType newType;
				if (lbType == LeaderboardTypeOption::Vector)
					newType = LeaderboardType::Vector;
				else if (lbType == LeaderboardTypeOption::Map)
					newType = LeaderboardType::Map;
				else
					newType = LeaderboardType::UnorderedMap;
				
				ChangeLeaderboardType(game.leaderboard, newType);
				
				StartPlayingState(game);
				
				// Apply difficulty settings
				DifficultyLevel diff = GetDifficulty(game.menu);
				if (diff == DifficultyLevel::Easy)
				{
					// Easy mode - slower speed
					SetPlayerSpeed(game.player, INITIAL_SPEED * 0.7f);
				}
				else if (diff == DifficultyLevel::Hard)
				{
					// Hard mode - faster speed
					SetPlayerSpeed(game.player, INITIAL_SPEED * 1.3f);
				}
				
				// Apply sound settings
				if (!IsSoundEnabled(game.menu))
				{
					game.eatAppleSound.setVolume(0);
					game.gameOverSound.setVolume(0);
				}
				else
				{
					game.eatAppleSound.setVolume(100);
					game.gameOverSound.setVolume(100);
				}
			}
		}
		else if (game.isPaused)
		{
			// Update pause menu
			UpdateMenu(game.menu, deltaTime);
			
			// Check if player wants to continue
			if (GetMenuState(game.menu) == MenuState::Playing)
			{
				game.isPaused = false;
			}
			// Check if player wants to exit to menu
			else if (GetMenuState(game.menu) == MenuState::MainMenu)
			{
				game.isPaused = false;
				game.isInMenu = true;
				game.isGameFinished = false;
				game.showLeaderboard = false;
			}
		}
		else
		{
			// Update game state
			if (!game.isGameFinished)
			{
				UpdatePlayingState(game, deltaTime);
			}
			else
			{
				UpdateGameoverState(game, deltaTime);
			}
		}
	}

	void DrawGame(Game& game, sf::RenderWindow& window)
	{
		// Draw menu or game
		if (game.isInMenu)
		{
			DrawMenu(game.menu, window);
			
			// If viewing leaderboard, draw it
			if (GetMenuState(game.menu) == MenuState::LeaderboardView)
			{
				DrawLeaderboard(game.leaderboard, window);
			}
			return;
		}
		
		// Draw background
		window.draw(game.background);
		
		// Draw game objects
		DrawPlayer(game.player, window);
		for (size_t i = 0; i < game.apples.size(); ++i)
		{
			DrawApple(game.apples[i], window);
		}

		for (size_t i = 0; i < game.rocks.size(); ++i)
		{
			DrawRock(game.rocks[i], window);
		}

		// Draw texts
		if (!game.isGameFinished)
		{
			window.draw(game.scoreText);
			window.draw(game.controlsHintText);
		}
		else
		{
			if (!game.showLeaderboard)
			{
				window.draw(game.gameOverText);
				window.draw(game.gameOverScoreText);
			}
			else
			{
				// Отображаем таблицу лидеров
				DrawLeaderboard(game.leaderboard, window);
				window.draw(game.restartHintText);
			}
		}

		// Draw pause menu
		if (game.isPaused)
		{
			// Dim background
			sf::RectangleShape dimBackground;
			dimBackground.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
			dimBackground.setFillColor(sf::Color(0, 0, 0, 150));
			window.draw(dimBackground);
			
			DrawMenu(game.menu, window);
		}

		// Draw exit dialog
		if (game.showExitDialog)
		{
			window.draw(game.exitDialogBackground);
			window.draw(game.exitDialogText);
			window.draw(game.exitDialogYesText);
			window.draw(game.exitDialogNoText);
		}
	}

	void DeinializeGame(Game& game)
	{

	}
}
