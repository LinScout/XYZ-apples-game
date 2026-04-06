#include "Leaderboard.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

namespace ApplesGame
{
	// Массив случайных имен для генерации
	const std::string RANDOM_NAMES[] = {
		"Alice", "Bob", "Carol", "Dave", "Eve",
		"Frank", "Grace", "Henry", "Ivy", "Jack",
		"Kate", "Leo", "Mary", "Nick", "Olivia"
	};
	const int NUM_RANDOM_NAMES = 15;

	std::string GetRandomName()
	{
		return RANDOM_NAMES[rand() % NUM_RANDOM_NAMES];
	}

	void InitLeaderboard(Leaderboard& leaderboard, sf::Font& font)
	{
		leaderboard.font = font;
		srand(static_cast<unsigned int>(time(nullptr)));
		GenerateRandomRecords(leaderboard);
	}

	void GenerateRandomRecords(Leaderboard& leaderboard)
	{
		leaderboard.records.clear();
		leaderboard.recordsMap.clear();

		// Генерируем 5-10 случайных записей
		int numRecords = 5 + rand() % 6; // От 5 до 10

		for (int i = 0; i < numRecords; ++i)
		{
			std::string name = GetRandomName();
			int score = rand() % 150; // Случайные очки от 0 до 149

			if (leaderboard.useMapVersion)
			{
				// Для map версии - проверяем уникальность имени
				while (leaderboard.recordsMap.find(name) != leaderboard.recordsMap.end())
				{
					name = GetRandomName();
				}
				leaderboard.recordsMap[name] = score;
			}
			else
			{
				// Для vector версии
				leaderboard.records.push_back({ name, score });
			}
		}

		// Сортируем vector версию
		if (!leaderboard.useMapVersion)
		{
			SortLeaderboard(leaderboard);
		}
	}

	void AddPlayerToLeaderboard(Leaderboard& leaderboard, const std::string& playerName, int score)
	{
		// Добавляем игрока в таблицу
		leaderboard.records.push_back({ playerName, score });

		// Сортируем таблицу вручную (insertion sort)
		SortLeaderboard(leaderboard);

		// Обновляем текстовые элементы
		UpdateLeaderboardTexts(leaderboard);
	}

	void SortLeaderboard(Leaderboard& leaderboard)
	{
		// Insertion Sort - эффективен для маленьких массивов
		// Сложность: O(n^2) в худшем случае, но O(n) для почти отсортированных данных
		// Для таблицы из 5-10 элементов это оптимальный выбор
		
		int n = leaderboard.records.size();
		for (int i = 1; i < n; ++i)
		{
			Record key = leaderboard.records[i];
			int j = i - 1;

			// Сортировка по убыванию очков
			while (j >= 0 && leaderboard.records[j].score < key.score)
			{
				leaderboard.records[j + 1] = leaderboard.records[j];
				j--;
			}
			leaderboard.records[j + 1] = key;
		}
	}

	void AddPlayerToLeaderboardMap(Leaderboard& leaderboard, const std::string& playerName, int score)
	{
		// Добавляем или обновляем запись игрока
		leaderboard.recordsMap[playerName] = score;

		// Обновляем текстовые элементы
		UpdateLeaderboardTexts(leaderboard);
	}

	void UpdateLeaderboardTexts(Leaderboard& leaderboard)
	{
		leaderboard.leaderboardTexts.clear();

		// Заголовок
		sf::Text headerText;
		headerText.setFont(leaderboard.font);
		headerText.setCharacterSize(30);
		headerText.setFillColor(sf::Color::Yellow);
		headerText.setString("===== LEADERBOARD =====");
		headerText.setPosition(250.f, 150.f);
		leaderboard.leaderboardTexts.push_back(headerText);

		float yPosition = 200.f;

		if (leaderboard.useMapVersion)
		{
			// Для map версии нужно скопировать в vector и отсортировать
			std::vector<Record> sortedRecords;
			for (const auto& pair : leaderboard.recordsMap)
			{
				sortedRecords.push_back({ pair.first, pair.second });
			}

			// Сортируем по убыванию очков
			for (size_t i = 1; i < sortedRecords.size(); ++i)
			{
				Record key = sortedRecords[i];
				int j = i - 1;
				while (j >= 0 && sortedRecords[j].score < key.score)
				{
					sortedRecords[j + 1] = sortedRecords[j];
					j--;
				}
				sortedRecords[j + 1] = key;
			}

			// Отображаем записи
			for (size_t i = 0; i < sortedRecords.size(); ++i)
			{
				sf::Text recordText;
				recordText.setFont(leaderboard.font);
				recordText.setCharacterSize(24);
				
				// Подсвечиваем игрока
				if (sortedRecords[i].name == "Player")
				{
					recordText.setFillColor(sf::Color::Green);
				}
				else
				{
					recordText.setFillColor(sf::Color::White);
				}

				// Форматируем строку с точками
				std::string dots = " ";
				int numDots = 20 - sortedRecords[i].name.length();
				for (int d = 0; d < numDots; ++d)
				{
					dots += ".";
				}

				std::string line = std::to_string(i + 1) + ". " + sortedRecords[i].name + dots + " " + std::to_string(sortedRecords[i].score);
				recordText.setString(line);
				recordText.setPosition(250.f, yPosition);
				yPosition += 30.f;

				leaderboard.leaderboardTexts.push_back(recordText);
			}
		}
		else
		{
			// Для vector версии просто отображаем
			for (size_t i = 0; i < leaderboard.records.size(); ++i)
			{
				sf::Text recordText;
				recordText.setFont(leaderboard.font);
				recordText.setCharacterSize(24);
				
				// Подсвечиваем игрока
				if (leaderboard.records[i].name == "Player")
				{
					recordText.setFillColor(sf::Color::Green);
				}
				else
				{
					recordText.setFillColor(sf::Color::White);
				}

				// Форматируем строку с точками
				std::string dots = " ";
				int numDots = 20 - leaderboard.records[i].name.length();
				for (int d = 0; d < numDots; ++d)
				{
					dots += ".";
				}

				std::string line = std::to_string(i + 1) + ". " + leaderboard.records[i].name + dots + " " + std::to_string(leaderboard.records[i].score);
				recordText.setString(line);
				recordText.setPosition(250.f, yPosition);
				yPosition += 30.f;

				leaderboard.leaderboardTexts.push_back(recordText);
			}
		}

		// Нижняя линия
		sf::Text footerText;
		footerText.setFont(leaderboard.font);
		footerText.setCharacterSize(30);
		footerText.setFillColor(sf::Color::Yellow);
		footerText.setString("=======================");
		footerText.setPosition(250.f, yPosition);
		leaderboard.leaderboardTexts.push_back(footerText);
	}

	void DrawLeaderboard(Leaderboard& leaderboard, sf::RenderWindow& window)
	{
		for (auto& text : leaderboard.leaderboardTexts)
		{
			window.draw(text);
		}
	}
}
