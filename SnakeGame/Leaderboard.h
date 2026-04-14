#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace SnakeGame
{
	struct LeaderboardEntry { std::string name; int score; };

	struct Leaderboard
	{
		std::vector<LeaderboardEntry> entries;
		sf::Font* font = nullptr;
		sf::Text titleText;
		std::vector<sf::Text> entryTexts;
	};

	void InitLeaderboard(Leaderboard& lb, sf::Font& font);
	void AddScore(Leaderboard& lb, const std::string& name, int score);
	void DrawLeaderboard(const Leaderboard& lb, sf::RenderWindow& window);
}
