#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Constants.h"
#include "Math.h"
#include "Player.h"
#include "Apple.h"
#include "Rock.h"
#include "Leaderboard.h"
#include "Menu.h"
#include <vector>

using namespace std;

namespace ApplesGame
{
	struct Game
	{
		Rectangle screenRect;
		
		Player player;
		vector<Apple> apples;
		vector<Rock> rocks;

		// Global game data
		int numEatenApples = 0;
		bool isGameFinished = false;
		float timeSinceGameFinish = 0.f;
		bool showLeaderboard = false;
		bool showExitDialog = false;
		float timeSinceLastKeyPress = 0.f;
		bool isInMenu = true;
		bool isPaused = false;

		// Leaderboard
		Leaderboard leaderboard;
		
		// Menu
		Menu menu;

		// Resources
		sf::Texture playerTexture;
		sf::Texture appleTexture;
		sf::Texture rockTexture;
		sf::SoundBuffer eatAppleSoundBuffer;
		sf::SoundBuffer gameOverSoundBuffer;

		// Game objects
		sf::Sound eatAppleSound;
		sf::Sound gameOverSound;
		sf::RectangleShape background;

		// Texts and fonts
		sf::Font font;
		sf::Text scoreText;
		sf::Text controlsHintText;
		sf::Text gameOverText;
		sf::Text gameOverScoreText;
		sf::Text restartHintText;
		sf::Text exitDialogText;
		sf::Text exitDialogYesText;
		sf::Text exitDialogNoText;
		sf::RectangleShape exitDialogBackground;
	};

	void InitGame(Game& game);
	void UpdateGame(Game& game, float deltaTime);
	void DrawGame(Game& game, sf::RenderWindow& window);
	void DeinializeGame(Game& game);
	bool HandleExitDialog(Game& game);
	bool ShouldCloseGame(const Game& game);

	void StartPlayingState(Game& game);
	void UpdatePlayingState(Game& game, float deltaTime);

	void StartGameoverState(Game& game);
	void UpdateGameoverState(Game& game, float deltaTime);
}
