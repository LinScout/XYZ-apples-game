#include "Leaderboard.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

namespace ApplesGame
{
	// Вектор случайных имен для генерации
	const vector<string> RANDOM_NAMES = {
		"Alice", "Bob", "Carol", "Dave", "Eve",
		"Frank", "Grace", "Henry", "Ivy", "Jack",
		"Kate", "Leo", "Mary", "Nick", "Olivia"
	};

	string GetRandomName()
	{
		return RANDOM_NAMES[rand() % RANDOM_NAMES.size()];
	}

	void InitLeaderboard(Leaderboard& leaderboard, sf::Font& font, LeaderboardType type)
	{
		leaderboard.font = font;
		leaderboard.currentType = type;
		GenerateRandomRecords(leaderboard);
		
		// Инициализация текста с информацией о типе
		leaderboard.typeInfoText.setFont(font);
		leaderboard.typeInfoText.setCharacterSize(18);
		leaderboard.typeInfoText.setFillColor(sf::Color::Cyan);
	}

	void GenerateRandomRecords(Leaderboard& leaderboard)
	{
		leaderboard.records.clear();
		leaderboard.recordsMap.clear();
		leaderboard.recordsUnorderedMap.clear();

		// Генерируем 5-10 случайных записей
		int numRecords = 5 + rand() % 6; // От 5 до 10

		for (int i = 0; i < numRecords; ++i)
		{
			string name = GetRandomName();
			int score = rand() % 150; // Случайные очки от 0 до 149

			if (leaderboard.currentType == LeaderboardType::Map)
			{
				// Для map версии - проверяем уникальность имени
				while (leaderboard.recordsMap.find(name) != leaderboard.recordsMap.end())
				{
					name = GetRandomName();
				}
				leaderboard.recordsMap[name] = score;
			}
			else if (leaderboard.currentType == LeaderboardType::UnorderedMap)
			{
				// Для unordered_map версии - проверяем уникальность имени
				while (leaderboard.recordsUnorderedMap.find(name) != leaderboard.recordsUnorderedMap.end())
				{
					name = GetRandomName();
				}
				leaderboard.recordsUnorderedMap[name] = score;
			}
			else // Vector
			{
				// Для vector версии
				leaderboard.records.push_back({ name, score });
			}
		}

		// Сортируем vector версию
		if (leaderboard.currentType == LeaderboardType::Vector)
		{
			SortLeaderboard(leaderboard);
		}
	}

	void AddPlayerToLeaderboard(Leaderboard& leaderboard, const string& playerName, int score)
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
		
		size_t n = leaderboard.records.size();
		for (size_t i = 1; i < n; ++i)
		{
			Record key = leaderboard.records[i];
			int j = static_cast<int>(i) - 1;

			// Сортировка по убыванию очков
			while (j >= 0 && leaderboard.records[j].score < key.score)
			{
				leaderboard.records[j + 1] = leaderboard.records[j];
				j--;
			}
			leaderboard.records[j + 1] = key;
		}
	}

	void AddPlayerToLeaderboardMap(Leaderboard& leaderboard, const string& playerName, int score)
	{
		// Добавляем или обновляем запись игрока
		// map: O(log n) для вставки/поиска
		// Автоматически сортируется по ключу (имени)
		leaderboard.recordsMap[playerName] = score;

		// Обновляем текстовые элементы
		UpdateLeaderboardTexts(leaderboard);
	}

	void AddPlayerToLeaderboardUnorderedMap(Leaderboard& leaderboard, const string& playerName, int score)
	{
		// Добавляем или обновляем запись игрока
		// unordered_map: O(1) average для вставки/поиска
		// НЕ сортируется автоматически
		leaderboard.recordsUnorderedMap[playerName] = score;

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
		headerText.setPosition(250.f, 100.f);
		leaderboard.leaderboardTexts.push_back(headerText);

		// Информация о типе реализации
		string typeInfo;
		if (leaderboard.currentType == LeaderboardType::Vector)
		{
			typeInfo = "Type: VECTOR + Insertion Sort";
		}
		else if (leaderboard.currentType == LeaderboardType::Map)
		{
			typeInfo = "Type: MAP (Red-Black Tree, O(log n))";
		}
		else
		{
			typeInfo = "Type: UNORDERED_MAP (Hash Table, O(1))";
		}
		
		leaderboard.typeInfoText.setString(typeInfo);
		leaderboard.typeInfoText.setPosition(200.f, 140.f);
		leaderboard.leaderboardTexts.push_back(leaderboard.typeInfoText);

		float yPosition = 180.f;

		if (leaderboard.currentType == LeaderboardType::Map)
		{
			// Для map версии нужно скопировать в vector и отсортировать по значению
			vector<Record> sortedRecords;
			for (const auto& pair : leaderboard.recordsMap)
			{
				sortedRecords.push_back({ pair.first, pair.second });
			}

			// Сортируем по убыванию очков (вручную)
			for (size_t i = 1; i < sortedRecords.size(); ++i)
			{
				Record key = sortedRecords[i];
				int j = static_cast<int>(i) - 1;
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
				string dots = " ";
				int numDots = 20 - static_cast<int>(sortedRecords[i].name.length());
				if (numDots < 0) numDots = 0;
				for (int d = 0; d < numDots; ++d)
				{
					dots += ".";
				}

				string line = to_string(i + 1) + ". " + sortedRecords[i].name + dots + " " + to_string(sortedRecords[i].score);
				recordText.setString(line);
				recordText.setPosition(250.f, yPosition);
				yPosition += 30.f;

				leaderboard.leaderboardTexts.push_back(recordText);
			}
		}
		else if (leaderboard.currentType == LeaderboardType::UnorderedMap)
		{
			// Для unordered_map версии нужно скопировать в vector и отсортировать
			vector<Record> sortedRecords;
			for (const auto& pair : leaderboard.recordsUnorderedMap)
			{
				sortedRecords.push_back({ pair.first, pair.second });
			}

			// Сортируем по убыванию очков (вручную)
			for (size_t i = 1; i < sortedRecords.size(); ++i)
			{
				Record key = sortedRecords[i];
				int j = static_cast<int>(i) - 1;
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
				string dots = " ";
				int numDots = 20 - static_cast<int>(sortedRecords[i].name.length());
				if (numDots < 0) numDots = 0;
				for (int d = 0; d < numDots; ++d)
				{
					dots += ".";
				}

				string line = to_string(i + 1) + ". " + sortedRecords[i].name + dots + " " + to_string(sortedRecords[i].score);
				recordText.setString(line);
				recordText.setPosition(250.f, yPosition);
				yPosition += 30.f;

				leaderboard.leaderboardTexts.push_back(recordText);
			}
		}
		else // Vector
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
				string dots = " ";
				int numDots = 20 - static_cast<int>(leaderboard.records[i].name.length());
				if (numDots < 0) numDots = 0;
				for (int d = 0; d < numDots; ++d)
				{
					dots += ".";
				}

				string line = to_string(i + 1) + ". " + leaderboard.records[i].name + dots + " " + to_string(leaderboard.records[i].score);
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

	void ChangeLeaderboardType(Leaderboard& leaderboard, LeaderboardType newType)
	{
		leaderboard.currentType = newType;
		GenerateRandomRecords(leaderboard);
	}
}
