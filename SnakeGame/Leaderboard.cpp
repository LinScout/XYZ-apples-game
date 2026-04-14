#include "Leaderboard.h"
#include "GameSettings.h"
#include <algorithm>

namespace SnakeGame
{
	void InitLeaderboard(Leaderboard& lb, sf::Font& font)
	{
		lb.font = &font;
		lb.entries.clear();
		lb.titleText.setFont(font);
		lb.titleText.setCharacterSize(40);
		lb.titleText.setFillColor(sf::Color::Yellow);
		lb.titleText.setString("LEADERBOARD");
		lb.titleText.setPosition(SCREEN_WIDTH / 2.f - 150.f, 50.f);
	}

	void AddScore(Leaderboard& lb, const std::string& name, int score)
	{
		lb.entries.push_back({ name, score });
		std::sort(lb.entries.begin(), lb.entries.end(),
			[](const LeaderboardEntry& a, const LeaderboardEntry& b) { return a.score > b.score; });
		if (lb.entries.size() > (size_t)MAX_RECORDS_TABLE_SIZE)
			lb.entries.resize(MAX_RECORDS_TABLE_SIZE);

		lb.entryTexts.clear();
		float yPos = 120.f;
		int rank = 1;
		for (const auto& e : lb.entries)
		{
			sf::Text t;
			t.setFont(*lb.font);
			t.setCharacterSize(24);
			t.setFillColor(sf::Color::White);
			t.setString(std::to_string(rank++) + ". " + e.name + ": " + std::to_string(e.score));
			t.setPosition(SCREEN_WIDTH / 2.f - 150.f, yPos);
			lb.entryTexts.push_back(t);
			yPos += 35.f;
		}
	}

	void DrawLeaderboard(const Leaderboard& lb, sf::RenderWindow& window)
	{
		window.draw(lb.titleText);
		for (const auto& t : lb.entryTexts)
			window.draw(t);
	}
}
