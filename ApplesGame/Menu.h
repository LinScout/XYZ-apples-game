#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
#include <string>

using namespace std;

namespace ApplesGame
{
	enum class MenuState
	{
		MainMenu,
		OptionsMenu,
		LeaderboardView,
		PauseMenu,
		Playing,
		Exiting
	};

	enum class DifficultyLevel
	{
		Easy,
		Normal,
		Hard
	};

	enum class LeaderboardTypeOption
	{
		Vector,
		Map,
		UnorderedMap
	};

	struct MenuItem
	{
		string text;
		sf::Text displayText;
		bool isSelected;
	};

	struct Menu
	{
		MenuState currentState;
		vector<MenuItem> mainMenuItems;
		vector<MenuItem> optionsMenuItems;
		vector<MenuItem> pauseMenuItems;
		int selectedMainIndex;
		int selectedOptionsIndex;
		int selectedPauseIndex;
		
		// Settings
		DifficultyLevel difficulty;
		bool soundEnabled;
		LeaderboardTypeOption leaderboardType;
		
		// Visual elements
		sf::Font font;
		sf::Text titleText;
		sf::RectangleShape background;
		
		// Input delay
		float timeSinceLastInput;
	};

	void InitMenu(Menu& menu, sf::Font& font);
	void UpdateMenu(Menu& menu, float deltaTime);
	void DrawMenu(Menu& menu, sf::RenderWindow& window);
	MenuState GetMenuState(const Menu& menu);
	void ResetMenu(Menu& menu);
	void ShowPauseMenu(Menu& menu);
	void ShowLeaderboardView(Menu& menu);
	
	// Getters for settings
	DifficultyLevel GetDifficulty(const Menu& menu);
	bool IsSoundEnabled(const Menu& menu);
	LeaderboardTypeOption GetLeaderboardType(const Menu& menu);
}
