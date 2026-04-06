#pragma once
#include <string>
#include <vector>
#include <map>
#include "SFML/Graphics.hpp"

namespace ApplesGame
{
	struct Record
	{
		std::string name;
		int score;
	};

	struct Leaderboard
	{
		// Вариант 1: Использование vector с ручной сортировкой
		std::vector<Record> records;

		// Вариант 2: Использование map для быстрого поиска по имени
		std::map<std::string, int> recordsMap;

		// Флаг для выбора варианта реализации
		bool useMapVersion = false;

		// Текстовые элементы для отображения
		sf::Font font;
		std::vector<sf::Text> leaderboardTexts;
	};

	// Инициализация таблицы лидеров
	void InitLeaderboard(Leaderboard& leaderboard, sf::Font& font);

	// Генерация случайных имен и очков
	void GenerateRandomRecords(Leaderboard& leaderboard);

	// Добавление игрока в таблицу (вариант с vector)
	void AddPlayerToLeaderboard(Leaderboard& leaderboard, const std::string& playerName, int score);

	// Ручная сортировка (insertion sort)
	void SortLeaderboard(Leaderboard& leaderboard);

	// Добавление игрока в таблицу (вариант с map)
	void AddPlayerToLeaderboardMap(Leaderboard& leaderboard, const std::string& playerName, int score);

	// Обновление текстовых элементов для отображения
	void UpdateLeaderboardTexts(Leaderboard& leaderboard);

	// Отрисовка таблицы лидеров
	void DrawLeaderboard(Leaderboard& leaderboard, sf::RenderWindow& window);

	// Получение случайного имени
	std::string GetRandomName();
}
