#pragma once
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include "SFML/Graphics.hpp"

using namespace std;

namespace ApplesGame
{
	enum class LeaderboardType
	{
		Vector,        // Вариант 1: vector с ручной сортировкой
		Map,           // Вариант 2: map (Red-Black Tree)
		UnorderedMap   // Вариант 3: unordered_map (Hash Table)
	};

	struct Record
	{
		string name;
		int score;
	};

	struct Leaderboard
	{
		// Вариант 1: Использование vector с ручной сортировкой
		vector<Record> records;

		// Вариант 2: Использование map (Red-Black Tree, O(log n))
		map<string, int> recordsMap;

		// Вариант 3: Использование unordered_map (Hash Table, O(1) average)
		unordered_map<string, int> recordsUnorderedMap;

		// Выбор типа реализации
		LeaderboardType currentType;

		// Текстовые элементы для отображения
		sf::Font font;
		vector<sf::Text> leaderboardTexts;
		sf::Text typeInfoText;
	};

	// Инициализация таблицы лидеров
	void InitLeaderboard(Leaderboard& leaderboard, sf::Font& font, LeaderboardType type);

	// Генерация случайных имен и очков
	void GenerateRandomRecords(Leaderboard& leaderboard);

	// Добавление игрока в таблицу (вариант с vector)
	void AddPlayerToLeaderboard(Leaderboard& leaderboard, const string& playerName, int score);

	// Ручная сортировка (insertion sort)
	void SortLeaderboard(Leaderboard& leaderboard);

	// Добавление игрока в таблицу (вариант с map)
	void AddPlayerToLeaderboardMap(Leaderboard& leaderboard, const string& playerName, int score);

	// Добавление игрока в таблицу (вариант с unordered_map)
	void AddPlayerToLeaderboardUnorderedMap(Leaderboard& leaderboard, const string& playerName, int score);

	// Обновление текстовых элементов для отображения
	void UpdateLeaderboardTexts(Leaderboard& leaderboard);

	// Отрисовка таблицы лидеров
	void DrawLeaderboard(Leaderboard& leaderboard, sf::RenderWindow& window);

	// Получение случайного имени
	string GetRandomName();
	
	// Смена типа таблицы лидеров
	void ChangeLeaderboardType(Leaderboard& leaderboard, LeaderboardType newType);
}
