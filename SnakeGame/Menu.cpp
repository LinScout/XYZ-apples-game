#include "Menu.h"
#include "GameSettings.h"
#include <SFML/Window/Keyboard.hpp>

namespace SnakeGame
{
	static float keyPressDelay = 0.f;
	const float KEY_PRESS_COOLDOWN = 0.2f;

	void InitMenu(Menu& menu, sf::Font& font)
	{
		menu.font = &font;
		menu.state = MenuState::Main;
		menu.selectedIndex = 0;
		menu.difficulty = DifficultyLevel::Normal;
		menu.soundEnabled = true;
		menu.infiniteApples = true;
		menu.settingsSelectedIndex = 0;

		menu.titleText.setFont(font);
		menu.titleText.setCharacterSize(60);
		menu.titleText.setFillColor(sf::Color::Green);
		menu.titleText.setString("SNAKE GAME");
		menu.titleText.setPosition(SCREEN_WIDTH / 2.f - 200.f, 100.f);

		menu.items.clear();
		float yPos = 250.f;
		for (const auto& text : std::vector<std::string>{"Play", "Settings", "Exit"})
		{
			MenuItem item;
			item.text = text;
			item.displayText.setFont(font);
			item.displayText.setCharacterSize(30);
			item.displayText.setFillColor(sf::Color::White);
			item.displayText.setString(text);
			item.displayText.setPosition(SCREEN_WIDTH / 2.f - 50.f, yPos);
			menu.items.push_back(item);
			yPos += 60.f;
		}
		menu.items[0].isSelected = true;
		menu.items[0].displayText.setFillColor(sf::Color::Yellow);

		menu.settingsItems.clear();
		yPos = 250.f;
		for (const auto& text : std::vector<std::string>{"Difficulty: Normal", "Sound: On", "Infinite Apples: On", "Back"})
		{
			MenuItem item;
			item.text = text;
			item.displayText.setFont(font);
			item.displayText.setCharacterSize(30);
			item.displayText.setFillColor(sf::Color::White);
			item.displayText.setString(text);
			item.displayText.setPosition(SCREEN_WIDTH / 2.f - 100.f, yPos);
			menu.settingsItems.push_back(item);
			yPos += 60.f;
		}
		menu.settingsItems[0].isSelected = true;
		menu.settingsItems[0].displayText.setFillColor(sf::Color::Yellow);
	}

	void UpdateMenu(Menu& menu, float deltaTime)
	{
		keyPressDelay += deltaTime;
		if (keyPressDelay < KEY_PRESS_COOLDOWN) return;

		auto navigate = [&](std::vector<MenuItem>& items, int& idx) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				items[idx].displayText.setFillColor(sf::Color::White);
				idx = (idx - 1 + (int)items.size()) % (int)items.size();
				items[idx].displayText.setFillColor(sf::Color::Yellow);
				keyPressDelay = 0.f;
				return true;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				items[idx].displayText.setFillColor(sf::Color::White);
				idx = (idx + 1) % (int)items.size();
				items[idx].displayText.setFillColor(sf::Color::Yellow);
				keyPressDelay = 0.f;
				return true;
			}
			return false;
		};

		if (menu.state == MenuState::Main)
		{
			navigate(menu.items, menu.selectedIndex);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			{
				if (menu.selectedIndex == 0) menu.state = MenuState::Playing;
				else if (menu.selectedIndex == 1) menu.state = MenuState::Settings;
				else menu.state = MenuState::Exiting;
				keyPressDelay = 0.f;
			}
		}
		else if (menu.state == MenuState::Settings)
		{
			navigate(menu.settingsItems, menu.settingsSelectedIndex);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			{
				if (menu.settingsSelectedIndex == 0)
				{
					if (menu.difficulty == DifficultyLevel::Easy) menu.difficulty = DifficultyLevel::Normal;
					else if (menu.difficulty == DifficultyLevel::Normal) menu.difficulty = DifficultyLevel::Hard;
					else menu.difficulty = DifficultyLevel::Easy;

					std::string s = "Difficulty: ";
					if (menu.difficulty == DifficultyLevel::Easy) s += "Easy";
					else if (menu.difficulty == DifficultyLevel::Normal) s += "Normal";
					else s += "Hard";
					menu.settingsItems[0].displayText.setString(s);
				}
				else if (menu.settingsSelectedIndex == 1)
				{
					menu.soundEnabled = !menu.soundEnabled;
					menu.settingsItems[1].displayText.setString(menu.soundEnabled ? "Sound: On" : "Sound: Off");
				}
				else if (menu.settingsSelectedIndex == 2)
				{
					menu.infiniteApples = !menu.infiniteApples;
					menu.settingsItems[2].displayText.setString(menu.infiniteApples ? "Infinite Apples: On" : "Infinite Apples: Off");
				}
				else menu.state = MenuState::Main;
				keyPressDelay = 0.f;
			}
		}
		else if (menu.state == MenuState::Paused)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
			{
				menu.state = MenuState::Playing;
				keyPressDelay = 0.f;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
			{
				menu.state = MenuState::Main;
				keyPressDelay = 0.f;
			}
		}
	}

	void DrawMenu(const Menu& menu, sf::RenderWindow& window)
	{
		window.draw(menu.titleText);

		if (menu.state == MenuState::Main)
		{
			for (const auto& item : menu.items)
				window.draw(item.displayText);
		}
		else if (menu.state == MenuState::Settings)
		{
			for (const auto& item : menu.settingsItems)
				window.draw(item.displayText);
		}
		else if (menu.state == MenuState::Paused)
		{
			sf::Text pauseText;
			pauseText.setFont(*menu.font);
			pauseText.setCharacterSize(50);
			pauseText.setFillColor(sf::Color::Yellow);
			pauseText.setString("PAUSED");
			pauseText.setPosition(SCREEN_WIDTH / 2.f - 100.f, SCREEN_HEIGHT / 2.f - 100.f);
			window.draw(pauseText);

			sf::Text hint;
			hint.setFont(*menu.font);
			hint.setCharacterSize(20);
			hint.setFillColor(sf::Color::White);
			hint.setString("P - continue  |  M - main menu");
			hint.setPosition(SCREEN_WIDTH / 2.f - 150.f, SCREEN_HEIGHT / 2.f);
			window.draw(hint);
		}
	}

	MenuState GetMenuState(const Menu& menu) { return menu.state; }
	DifficultyLevel GetDifficulty(const Menu& menu) { return menu.difficulty; }
	bool IsSoundEnabled(const Menu& menu) { return menu.soundEnabled; }
	bool GetInfiniteApples(const Menu& menu) { return menu.infiniteApples; }

	void ShowPauseMenu(Menu& menu) { menu.state = MenuState::Paused; }

	void ResetMenu(Menu& menu)
	{
		menu.state = MenuState::Main;
		menu.selectedIndex = 0;
		for (size_t i = 0; i < menu.items.size(); ++i)
			menu.items[i].displayText.setFillColor(i == 0 ? sf::Color::Yellow : sf::Color::White);
	}
}
