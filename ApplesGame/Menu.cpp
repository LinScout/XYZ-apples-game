#include "Menu.h"
#include "Constants.h"

using namespace std;

namespace ApplesGame
{
	const float INPUT_DELAY = 0.15f;

	void InitMenu(Menu& menu, sf::Font& font)
	{
		menu.font = font;
		menu.currentState = MenuState::MainMenu;
		menu.selectedMainIndex = 0;
		menu.selectedOptionsIndex = 0;
		menu.selectedPauseIndex = 0;
		menu.timeSinceLastInput = 0.f;
		
		// Default settings
		menu.difficulty = DifficultyLevel::Normal;
		menu.soundEnabled = true;
		menu.leaderboardType = LeaderboardTypeOption::Vector;
		
		// Background
		menu.background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		menu.background.setFillColor(sf::Color(20, 20, 40));
		
		// Title
		menu.titleText.setFont(menu.font);
		menu.titleText.setCharacterSize(80);
		menu.titleText.setFillColor(sf::Color::Yellow);
		menu.titleText.setString("APPLES GAME");
		menu.titleText.setPosition(SCREEN_WIDTH / 2.f - 250.f, 50.f);
		
		// Main menu items
		menu.mainMenuItems.clear();
		menu.mainMenuItems.push_back({"Play", sf::Text(), true});
		menu.mainMenuItems.push_back({"Leaderboard", sf::Text(), false});
		menu.mainMenuItems.push_back({"Options", sf::Text(), false});
		menu.mainMenuItems.push_back({"Exit", sf::Text(), false});
		
		float yPos = 250.f;
		for (size_t i = 0; i < menu.mainMenuItems.size(); ++i)
		{
			menu.mainMenuItems[i].displayText.setFont(menu.font);
			menu.mainMenuItems[i].displayText.setCharacterSize(40);
			menu.mainMenuItems[i].displayText.setString(menu.mainMenuItems[i].text);
			menu.mainMenuItems[i].displayText.setPosition(SCREEN_WIDTH / 2.f - 80.f, yPos);
			yPos += 80.f;
		}
		
		// Options menu items
		menu.optionsMenuItems.clear();
		menu.optionsMenuItems.push_back({"Difficulty: Normal", sf::Text(), true});
		menu.optionsMenuItems.push_back({"Sound: ON", sf::Text(), false});
		menu.optionsMenuItems.push_back({"Leaderboard: Vector", sf::Text(), false});
		menu.optionsMenuItems.push_back({"Back", sf::Text(), false});
		
		yPos = 250.f;
		for (size_t i = 0; i < menu.optionsMenuItems.size(); ++i)
		{
			menu.optionsMenuItems[i].displayText.setFont(menu.font);
			menu.optionsMenuItems[i].displayText.setCharacterSize(40);
			menu.optionsMenuItems[i].displayText.setString(menu.optionsMenuItems[i].text);
			menu.optionsMenuItems[i].displayText.setPosition(SCREEN_WIDTH / 2.f - 200.f, yPos);
			yPos += 80.f;
		}
		
		// Pause menu items
		menu.pauseMenuItems.clear();
		menu.pauseMenuItems.push_back({"Continue", sf::Text(), true});
		menu.pauseMenuItems.push_back({"Exit to Menu", sf::Text(), false});
		
		yPos = 250.f;
		for (size_t i = 0; i < menu.pauseMenuItems.size(); ++i)
		{
			menu.pauseMenuItems[i].displayText.setFont(menu.font);
			menu.pauseMenuItems[i].displayText.setCharacterSize(40);
			menu.pauseMenuItems[i].displayText.setString(menu.pauseMenuItems[i].text);
			menu.pauseMenuItems[i].displayText.setPosition(SCREEN_WIDTH / 2.f - 120.f, yPos);
			yPos += 80.f;
		}
	}

	void UpdateMainMenu(Menu& menu)
	{
		// Navigation
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			if (menu.timeSinceLastInput >= INPUT_DELAY)
			{
				menu.mainMenuItems[menu.selectedMainIndex].isSelected = false;
				menu.selectedMainIndex--;
				if (menu.selectedMainIndex < 0)
					menu.selectedMainIndex = menu.mainMenuItems.size() - 1;
				menu.mainMenuItems[menu.selectedMainIndex].isSelected = true;
				menu.timeSinceLastInput = 0.f;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			if (menu.timeSinceLastInput >= INPUT_DELAY)
			{
				menu.mainMenuItems[menu.selectedMainIndex].isSelected = false;
				menu.selectedMainIndex++;
				if (menu.selectedMainIndex >= (int)menu.mainMenuItems.size())
					menu.selectedMainIndex = 0;
				menu.mainMenuItems[menu.selectedMainIndex].isSelected = true;
				menu.timeSinceLastInput = 0.f;
			}
		}
		
		// Selection
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			if (menu.timeSinceLastInput >= INPUT_DELAY)
			{
				if (menu.selectedMainIndex == 0) // Play
				{
					menu.currentState = MenuState::Playing;
				}
				else if (menu.selectedMainIndex == 1) // Leaderboard
				{
					menu.currentState = MenuState::LeaderboardView;
				}
				else if (menu.selectedMainIndex == 2) // Options
				{
					menu.currentState = MenuState::OptionsMenu;
					menu.selectedOptionsIndex = 0;
					for (size_t i = 0; i < menu.optionsMenuItems.size(); ++i)
					{
						menu.optionsMenuItems[i].isSelected = (i == 0);
					}
				}
				else if (menu.selectedMainIndex == 3) // Exit
				{
					menu.currentState = MenuState::Exiting;
				}
				menu.timeSinceLastInput = 0.f;
			}
		}
	}

	void UpdateOptionsMenu(Menu& menu)
	{
		// Navigation
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			if (menu.timeSinceLastInput >= INPUT_DELAY)
			{
				menu.optionsMenuItems[menu.selectedOptionsIndex].isSelected = false;
				menu.selectedOptionsIndex--;
				if (menu.selectedOptionsIndex < 0)
					menu.selectedOptionsIndex = menu.optionsMenuItems.size() - 1;
				menu.optionsMenuItems[menu.selectedOptionsIndex].isSelected = true;
				menu.timeSinceLastInput = 0.f;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			if (menu.timeSinceLastInput >= INPUT_DELAY)
			{
				menu.optionsMenuItems[menu.selectedOptionsIndex].isSelected = false;
				menu.selectedOptionsIndex++;
				if (menu.selectedOptionsIndex >= (int)menu.optionsMenuItems.size())
					menu.selectedOptionsIndex = 0;
				menu.optionsMenuItems[menu.selectedOptionsIndex].isSelected = true;
				menu.timeSinceLastInput = 0.f;
			}
		}
		
		// Selection
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || 
		    sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || 
		    sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			if (menu.timeSinceLastInput >= INPUT_DELAY)
			{
				if (menu.selectedOptionsIndex == 0) // Difficulty
				{
					// Cycle through difficulties
					if (menu.difficulty == DifficultyLevel::Easy)
						menu.difficulty = DifficultyLevel::Normal;
					else if (menu.difficulty == DifficultyLevel::Normal)
						menu.difficulty = DifficultyLevel::Hard;
					else
						menu.difficulty = DifficultyLevel::Easy;
					
					// Update text
					string diffText = "Difficulty: ";
					if (menu.difficulty == DifficultyLevel::Easy)
						diffText += "Easy";
					else if (menu.difficulty == DifficultyLevel::Normal)
						diffText += "Normal";
					else
						diffText += "Hard";
					
					menu.optionsMenuItems[0].text = diffText;
					menu.optionsMenuItems[0].displayText.setString(diffText);
				}
				else if (menu.selectedOptionsIndex == 1) // Sound
				{
					menu.soundEnabled = !menu.soundEnabled;
					string soundText = "Sound: ";
					soundText += menu.soundEnabled ? "ON" : "OFF";
					menu.optionsMenuItems[1].text = soundText;
					menu.optionsMenuItems[1].displayText.setString(soundText);
				}
				else if (menu.selectedOptionsIndex == 2) // Leaderboard Type
				{
					// Cycle through leaderboard types
					if (menu.leaderboardType == LeaderboardTypeOption::Vector)
						menu.leaderboardType = LeaderboardTypeOption::Map;
					else if (menu.leaderboardType == LeaderboardTypeOption::Map)
						menu.leaderboardType = LeaderboardTypeOption::UnorderedMap;
					else
						menu.leaderboardType = LeaderboardTypeOption::Vector;
					
					// Update text
					string lbText = "Leaderboard: ";
					if (menu.leaderboardType == LeaderboardTypeOption::Vector)
						lbText += "Vector";
					else if (menu.leaderboardType == LeaderboardTypeOption::Map)
						lbText += "Map";
					else
						lbText += "UnorderedMap";
					
					menu.optionsMenuItems[2].text = lbText;
					menu.optionsMenuItems[2].displayText.setString(lbText);
				}
				else if (menu.selectedOptionsIndex == 3) // Back
				{
					menu.currentState = MenuState::MainMenu;
					menu.selectedMainIndex = 0;
					for (size_t i = 0; i < menu.mainMenuItems.size(); ++i)
					{
						menu.mainMenuItems[i].isSelected = (i == 0);
					}
				}
				menu.timeSinceLastInput = 0.f;
			}
		}
		
		// ESC to go back
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			if (menu.timeSinceLastInput >= INPUT_DELAY)
			{
				menu.currentState = MenuState::MainMenu;
				menu.selectedMainIndex = 0;
				for (size_t i = 0; i < menu.mainMenuItems.size(); ++i)
				{
					menu.mainMenuItems[i].isSelected = (i == 0);
				}
				menu.timeSinceLastInput = 0.f;
			}
		}
	}

	void UpdateLeaderboardView(Menu& menu)
	{
		// Backspace to go back
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
		{
			if (menu.timeSinceLastInput >= INPUT_DELAY)
			{
				menu.currentState = MenuState::MainMenu;
				menu.selectedMainIndex = 0;
				for (size_t i = 0; i < menu.mainMenuItems.size(); ++i)
				{
					menu.mainMenuItems[i].isSelected = (i == 0);
				}
				menu.timeSinceLastInput = 0.f;
			}
		}
	}

	void UpdatePauseMenu(Menu& menu)
	{
		// Navigation
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			if (menu.timeSinceLastInput >= INPUT_DELAY)
			{
				menu.pauseMenuItems[menu.selectedPauseIndex].isSelected = false;
				menu.selectedPauseIndex--;
				if (menu.selectedPauseIndex < 0)
					menu.selectedPauseIndex = menu.pauseMenuItems.size() - 1;
				menu.pauseMenuItems[menu.selectedPauseIndex].isSelected = true;
				menu.timeSinceLastInput = 0.f;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			if (menu.timeSinceLastInput >= INPUT_DELAY)
			{
				menu.pauseMenuItems[menu.selectedPauseIndex].isSelected = false;
				menu.selectedPauseIndex++;
				if (menu.selectedPauseIndex >= (int)menu.pauseMenuItems.size())
					menu.selectedPauseIndex = 0;
				menu.pauseMenuItems[menu.selectedPauseIndex].isSelected = true;
				menu.timeSinceLastInput = 0.f;
			}
		}
		
		// Selection
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			if (menu.timeSinceLastInput >= INPUT_DELAY)
			{
				if (menu.selectedPauseIndex == 0) // Continue
				{
					menu.currentState = MenuState::Playing;
				}
				else if (menu.selectedPauseIndex == 1) // Exit to Menu
				{
					menu.currentState = MenuState::MainMenu;
					menu.selectedMainIndex = 0;
					for (size_t i = 0; i < menu.mainMenuItems.size(); ++i)
					{
						menu.mainMenuItems[i].isSelected = (i == 0);
					}
				}
				menu.timeSinceLastInput = 0.f;
			}
		}
	}

	void UpdateMenu(Menu& menu, float deltaTime)
	{
		menu.timeSinceLastInput += deltaTime;
		
		if (menu.currentState == MenuState::MainMenu)
		{
			UpdateMainMenu(menu);
		}
		else if (menu.currentState == MenuState::OptionsMenu)
		{
			UpdateOptionsMenu(menu);
		}
		else if (menu.currentState == MenuState::LeaderboardView)
		{
			UpdateLeaderboardView(menu);
		}
		else if (menu.currentState == MenuState::PauseMenu)
		{
			UpdatePauseMenu(menu);
		}
	}

	void DrawMenu(Menu& menu, sf::RenderWindow& window)
	{
		window.draw(menu.background);
		window.draw(menu.titleText);
		
		if (menu.currentState == MenuState::MainMenu)
		{
			for (size_t i = 0; i < menu.mainMenuItems.size(); ++i)
			{
				if (menu.mainMenuItems[i].isSelected)
				{
					menu.mainMenuItems[i].displayText.setFillColor(sf::Color::Yellow);
					menu.mainMenuItems[i].displayText.setString("> " + menu.mainMenuItems[i].text + " <");
				}
				else
				{
					menu.mainMenuItems[i].displayText.setFillColor(sf::Color::White);
					menu.mainMenuItems[i].displayText.setString(menu.mainMenuItems[i].text);
				}
				window.draw(menu.mainMenuItems[i].displayText);
			}
		}
		else if (menu.currentState == MenuState::OptionsMenu)
		{
			for (size_t i = 0; i < menu.optionsMenuItems.size(); ++i)
			{
				if (menu.optionsMenuItems[i].isSelected)
				{
					menu.optionsMenuItems[i].displayText.setFillColor(sf::Color::Yellow);
					menu.optionsMenuItems[i].displayText.setString("> " + menu.optionsMenuItems[i].text + " <");
				}
				else
				{
					menu.optionsMenuItems[i].displayText.setFillColor(sf::Color::White);
					menu.optionsMenuItems[i].displayText.setString(menu.optionsMenuItems[i].text);
				}
				window.draw(menu.optionsMenuItems[i].displayText);
			}
			
			// Draw hint
			sf::Text hintText;
			hintText.setFont(menu.font);
			hintText.setCharacterSize(20);
			hintText.setFillColor(sf::Color(150, 150, 150));
			hintText.setString("Use arrows to navigate, Enter to select, ESC to go back");
			hintText.setPosition(100.f, SCREEN_HEIGHT - 50.f);
			window.draw(hintText);
		}
		else if (menu.currentState == MenuState::LeaderboardView)
		{
			// Hint for leaderboard view
			sf::Text hintText;
			hintText.setFont(menu.font);
			hintText.setCharacterSize(20);
			hintText.setFillColor(sf::Color(150, 150, 150));
			hintText.setString("Press Backspace to return to menu");
			hintText.setPosition(220.f, SCREEN_HEIGHT - 50.f);
			window.draw(hintText);
		}
		else if (menu.currentState == MenuState::PauseMenu)
		{
			// Draw pause menu title
			sf::Text pauseTitle;
			pauseTitle.setFont(menu.font);
			pauseTitle.setCharacterSize(60);
			pauseTitle.setFillColor(sf::Color::Yellow);
			pauseTitle.setString("PAUSED");
			pauseTitle.setPosition(SCREEN_WIDTH / 2.f - 120.f, 100.f);
			window.draw(pauseTitle);
			
			// Draw pause menu items
			for (size_t i = 0; i < menu.pauseMenuItems.size(); ++i)
			{
				if (menu.pauseMenuItems[i].isSelected)
				{
					menu.pauseMenuItems[i].displayText.setFillColor(sf::Color::Yellow);
					menu.pauseMenuItems[i].displayText.setString("> " + menu.pauseMenuItems[i].text + " <");
				}
				else
				{
					menu.pauseMenuItems[i].displayText.setFillColor(sf::Color::White);
					menu.pauseMenuItems[i].displayText.setString(menu.pauseMenuItems[i].text);
				}
				window.draw(menu.pauseMenuItems[i].displayText);
			}
		}
	}

	MenuState GetMenuState(const Menu& menu)
	{
		return menu.currentState;
	}

	void ResetMenu(Menu& menu)
	{
		menu.currentState = MenuState::MainMenu;
		menu.selectedMainIndex = 0;
		for (size_t i = 0; i < menu.mainMenuItems.size(); ++i)
		{
			menu.mainMenuItems[i].isSelected = (i == 0);
		}
	}

	DifficultyLevel GetDifficulty(const Menu& menu)
	{
		return menu.difficulty;
	}

	bool IsSoundEnabled(const Menu& menu)
	{
		return menu.soundEnabled;
	}

	LeaderboardTypeOption GetLeaderboardType(const Menu& menu)
	{
		return menu.leaderboardType;
	}

	void ShowPauseMenu(Menu& menu)
	{
		menu.currentState = MenuState::PauseMenu;
		menu.selectedPauseIndex = 0;
		for (size_t i = 0; i < menu.pauseMenuItems.size(); ++i)
		{
			menu.pauseMenuItems[i].isSelected = (i == 0);
		}
	}

	void ShowLeaderboardView(Menu& menu)
	{
		menu.currentState = MenuState::LeaderboardView;
	}
}
