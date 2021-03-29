#ifndef GAME_H_
#define GAME_H_

#include <vector>
#include <list>
#include <queue>
#include <unordered_map>
#include <string>
#include <random>
#include "Button.h"


struct Game_Status
{
	int points;
	bool result;
	Game_Status(int p, bool r) : points(p), result(r) {}
};

class Game
{
private:
	std::unique_ptr<sf::RenderWindow> Window;
	std::vector<std::vector<std::shared_ptr<Mine_Button>>> Buttons;
	std::unordered_map<std::string, std::shared_ptr<sf::Image>> Image_Map;
	int score;
	int max_score;
	bool Game_Lost;
	std::string Setting_mines(int max_places_y, int max_places_x, int max_mine);
	std::string Setting_places(int max_places_y, int max_places_x);
	std::string Setting_Mine_Count();
	std::string Left_Mouse_Pressed(Mine_Button& ref_Button, int indx_y, int indx_x);
	std::string Right_Mouse_Pressed(Mine_Button& ref_Button);
public:

	enum class Difficulty
	{
		Easy,
		Normal,
		Hard
	};

	Game();
	std::string Setting_up(Difficulty d);
	Game_Status Begin();
	std::string Reset_Game();
	std::vector<std::shared_ptr<Mine_Button> > operator[](int i) { return Buttons[i]; }
	bool is_Game_Lost()
	{
		return Game_Lost;
	}
};
#endif // !GAME_H_
#pragma once
	