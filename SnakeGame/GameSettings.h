#pragma once
#include <string>

namespace SnakeGame
{
	const std::string RESOURCES_PATH      = "Resources/";
	const std::string TEXTURES_PATH       = RESOURCES_PATH + "Textures/";
	const std::string FONTS_PATH          = RESOURCES_PATH + "Fonts/";

	const int SCREEN_WIDTH  = 800;
	const int SCREEN_HEIGHT = 600;

	const int GRID_SIZE   = 20;
	const int GRID_WIDTH  = SCREEN_WIDTH  / GRID_SIZE;
	const int GRID_HEIGHT = SCREEN_HEIGHT / GRID_SIZE;

	const float INITIAL_SPEED    = 0.15f;
	const float SPEED_INCREASE   = 0.005f;
	const float MIN_SPEED        = 0.05f;

	const int   INITIAL_SNAKE_LENGTH    = 3;
	const int   MAX_APPLES              = 80;
	const float PAUSE_LENGTH            = 2.f;
	const float TIME_PER_FRAME          = 1.f / 60.f;

	const int MAX_RECORDS_TABLE_SIZE = 5;
}
