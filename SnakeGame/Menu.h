#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace SnakeGame
{
	enum class MenuState { Main, Settings, Playing, Paused, Exiting };
	enum class DifficultyLevel { Easy, Normal, Hard };

	struct MenuItem
	{
		std::string text;
		sf::Text displayText;
		bool isSelected = false;
	};

	struct Menu
	{
		MenuState state = MenuState::Main;
		std::vector<MenuItem> items;
		int selectedIndex = 0;
		sf::Font* font = nullptr;
		sf::Text titleText;

		DifficultyLevel difficulty = DifficultyLevel::Normal;
		bool soundEnabled = true;
		bool infiniteApples = true;

		std::vector<MenuItem> settingsItems;
		int settingsSelectedIndex = 0;
	};

	void InitMenu(Menu& menu, sf::Font& font);
	void UpdateMenu(Menu& menu, float deltaTime);
	void DrawMenu(const Menu& menu, sf::RenderWindow& window);
	MenuState GetMenuState(const Menu& menu);
	DifficultyLevel GetDifficulty(const Menu& menu);
	bool IsSoundEnabled(const Menu& menu);
	bool GetInfiniteApples(const Menu& menu);
	void ShowPauseMenu(Menu& menu);
	void ResetMenu(Menu& menu);
}
