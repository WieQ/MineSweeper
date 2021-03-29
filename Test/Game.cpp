#include "Game.h"
#include <iostream>
Game::Game()
{
	//setting every Image in game and assigning them to Map
	//for reducing Ram usage we are using pointers so every Image
	//is setted up once
	
	std::shared_ptr<sf::Image> Button_Image(new sf::Image);
	Button_Image->loadFromFile("Sprite\\Button.png");
	Image_Map["Button"] = Button_Image;
	
	Button_Image = std::shared_ptr<sf::Image>(new sf::Image);
	Button_Image->loadFromFile("Sprite\\Mine.png");
	Image_Map["Mine"] = Button_Image;

	Button_Image = std::shared_ptr<sf::Image>(new sf::Image);
	Button_Image->loadFromFile("Sprite\\Flag_R.png");
	Image_Map["Flag_R"] = Button_Image;

	Button_Image = std::shared_ptr<sf::Image>(new sf::Image);
	Button_Image->loadFromFile("Sprite\\Flag_L.png");
	Image_Map["Flag_L"] = Button_Image;

	const int max_mine_around = 9;
	for (unsigned int i = 0; i < max_mine_around; ++i)
	{
		Button_Image = std::shared_ptr<sf::Image>(new sf::Image);
		Button_Image->loadFromFile("Sprite\\" + std::to_string(i) + ".png");
		Image_Map[std::to_string(i)] = Button_Image;
	}
	score = 0;
	max_score = 0;
	Game_Lost = false;
}

std::string Game::Setting_up(Game::Difficulty d)
{
	std::string Status = "Completed";
	//we are starting by creating 3 variables
	//they will help us to setup Difficulty
	auto max_mine = 0;
	auto max_places_y = 0;
	auto max_places_x = 0;
	
	//By using enum we choose which difficulty we want to setup
	switch (d)
	{
	case Game::Difficulty::Easy:
		max_mine = 10;
		max_places_y = 10;
		max_places_x = 10;
		break;
	case Game::Difficulty::Normal:
		max_mine = 40;
		max_places_y = 20;
		max_places_x = 20;
		break;
	case Game::Difficulty::Hard:
		max_mine = 99;
		max_places_y = 16;
		max_places_x = 30;
		break;
	}
	max_score = max_places_x * max_places_y - max_mine;
	//Creating Window based on difficulty so we can fit every button
	Window = std::move(std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(25 * max_places_x, 25 * max_places_y), "Saper")));
	//Calling Setting_places, Method that setup every button on right place
	this->Setting_places(max_places_y, max_places_x);
	//Calling Setting_mines, Method that will setup mines based on difficulty
	this->Setting_mines(max_places_y, max_places_x, max_mine);
	//Calling Setting_Mine_Count, Method that will setup every button it finds mines and then 
	//increment mine value on each Mine_Button around
	this->Setting_Mine_Count();
	return Status;
}

//Method that will start Game it's made as diffrent method so we can load game before showing it
Game_Status Game::Begin()
{
	std::string Status = "Begin() Completed!";
	Window->setVisible(true);
	//Main loop which will work till Window is Opened
	while (Window->isOpen())
	{
		//event variable for hangling new event e.g. Mouse Clicking
		sf::Event event;
		//Event Loop working till there is event to handle 
		while (Window->pollEvent(event))
		{
			//if user event type  
			if (event.type == sf::Event::Closed)
				Window->close();
			if (event.type == sf::Event::MouseButtonPressed)
			{
				for (int indx_y = 0; indx_y < Buttons.size(); indx_y++)
				{
					for (int indx_x = 0; indx_x < Buttons[indx_y].size(); ++indx_x)
					{
						Mine_Button& ref_Button = *Buttons[indx_y][indx_x];
						if (ref_Button.GetSprite().getGlobalBounds().contains(Window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)))) {
							if (event.mouseButton.button == sf::Mouse::Left && !ref_Button.is_Flagged())
								this->Left_Mouse_Pressed(ref_Button,indx_y,indx_x);
							else if (event.mouseButton.button == sf::Mouse::Right)
							{
								this->Right_Mouse_Pressed(ref_Button);
							}
						}
					}
				}
			}
		}
		
		if (score == max_score || is_Game_Lost())
		{
			for (auto indx_y = 0; indx_y < Buttons.size(); ++indx_y)
			{
				for (auto indx_x = 0; indx_x < Buttons[indx_y].size(); ++indx_x)
				{
					Mine_Button& ref_Button = *Buttons[indx_y][indx_x];
					if (ref_Button.is_Mine() && ref_Button.is_Active())
					{
						ref_Button.Clicked();
					}
					Window->draw(ref_Button.GetSprite());
				}
			}
			Window->display();
			return Game_Status(score, is_Game_Lost());
		}

		Window->clear();
		for (int i = 0; i < Buttons.size(); ++i)
		{
			for (int j = 0; j < Buttons[i].size(); ++j)
			{
				Window->draw(Buttons[i][j]->GetSprite());
			}
		}
		Window->display();

	}
	return Game_Status(score, is_Game_Lost());
}
std::string Game::Setting_places(int max_places_y, int max_places_x)
{
	std::string Status = "Setting_places() Completed!";
	{
		auto x = 25;
		auto y = 25;
		for (int i = 0; i < max_places_y; ++i)
		{
			std::vector<std::shared_ptr<Mine_Button>> temp_Button;
			for (int j = 0; j < max_places_x; ++j)
			{
				std::shared_ptr<Mine_Button> temp(new Mine_Button);
				temp->Set_Active_Texture(Image_Map["Button"]);
				temp->Set_Flagged_Image(Image_Map["Flag_R"]);
				temp->GetSprite().move(sf::Vector2f(x * j, y * i));
				temp_Button.push_back(temp);
			}
			Buttons.push_back(temp_Button);
			temp_Button.clear();
		}
	}	
	return Status;
}

std::string Game::Setting_mines(int max_places_y, int max_places_x, int max_mine)
{
	std::string Status = "Setting_mines() Completed!";
	std::random_device random_helper;
	std::uniform_int_distribution<int> dist_y(0, max_places_y - 1);
	std::uniform_int_distribution<int> dist_x(0, max_places_x - 1);
	std::list<std::pair<int, int>> duplicates;
	int current_mines = 0;
	while (current_mines < max_mine)
	{
		auto mine_indx_y = dist_y(random_helper);
		auto mine_indx_x = dist_x(random_helper);

		auto it = std::find(duplicates.begin(), duplicates.end(), std::pair<int, int>(mine_indx_y, mine_indx_x));
		if (it == duplicates.end())
		{
			Mine_Button& ref_Button = *Buttons[mine_indx_y][mine_indx_x];
			ref_Button.Set_After_Click_Image(Image_Map["Mine"]);
			ref_Button.Set_mine(-1);
			duplicates.push_back(std::pair<int, int>(mine_indx_y, mine_indx_x));
			current_mines++;
		}
	}
	return Status;
}

std::string Game::Setting_Mine_Count()
{
	std::string Status = "Setting_Mine_Count() Completed!";

	for (int indx_y = 0; indx_y < Buttons.size(); ++indx_y)
	{
		for (int indx_x = 0; indx_x < Buttons[indx_y].size(); ++indx_x)
		{
			Mine_Button& ref_Button = *Buttons[indx_y][indx_x];
			if (ref_Button.is_Mine())
			{
				for (int neighbour_y_to_check = -1; neighbour_y_to_check < 2; ++neighbour_y_to_check)
				{
					auto neighbour_y = indx_y + neighbour_y_to_check;
					if (neighbour_y >= 0 && neighbour_y < Buttons.size())
					{
						for (int neighbour_x_to_check = -1; neighbour_x_to_check < 2; ++neighbour_x_to_check)
						{
							auto neighbour_x = indx_x + neighbour_x_to_check;
							if (neighbour_x >= 0 && neighbour_x < Buttons[indx_y].size())
							{
								Mine_Button& ref_neighbour_Button = *Buttons[neighbour_y][neighbour_x];
								if (!ref_neighbour_Button.is_Mine())
									ref_neighbour_Button.Set_mine(ref_neighbour_Button.GetMine() + 1);
							}
						}
					}
				}
			}
		}
	}

	for (int indx_y = 0; indx_y < Buttons.size(); ++indx_y)
	{
		for (int indx_x = 0; indx_x < Buttons[indx_y].size(); ++indx_x)
		{
			Mine_Button& ref_Button = *Buttons[indx_y][indx_x];
			if (!ref_Button.is_Mine())
			{
				ref_Button.Set_After_Click_Image(Image_Map[std::to_string(ref_Button.GetMine())]);
			}
		}
	}
	return Status;
}

std::string Game::Left_Mouse_Pressed(Mine_Button& ref_Button, int indx_y, int indx_x)
{
	std::string Status = "Left_Mouse_Pressed() Completed";
	auto point = ref_Button.Clicked();
	if (point == 1)
		score += point;
	else
	{
		Game_Lost = true;
		return Status;
	}
		//Game_Over
	if (ref_Button.GetMine() == 0)
	{
		std::queue<std::pair<int, int>> Places_To_check;
		Places_To_check.push(std::pair<int, int>(indx_y, indx_x));
		while (!Places_To_check.empty())
		{
			std::pair<int, int> Place = Places_To_check.front();
			Places_To_check.pop();
			for (int neighbour_y_to_check = -1; neighbour_y_to_check < 2; ++neighbour_y_to_check)
			{
				auto neighbour_y = neighbour_y_to_check + Place.first;
				if (neighbour_y >= 0 && neighbour_y < Buttons.size())
				{
					for (int neighbour_x_to_check = -1; neighbour_x_to_check < 2; ++neighbour_x_to_check)
					{
						auto neighbour_x = neighbour_x_to_check + Place.second;
						if (neighbour_x >= 0 && neighbour_x < Buttons[Place.first].size())
						{
							Mine_Button& ref_neighbour_Button = *Buttons[neighbour_y][neighbour_x];
							if (ref_neighbour_Button.is_Active())
							{
								auto point = ref_neighbour_Button.Clicked();
								if (point == 1)
									score += point;
								if (ref_neighbour_Button.GetMine() == 0)
								{
									Places_To_check.push(std::pair<int, int>(neighbour_y, neighbour_x));
								}
							}
						}
					}
				}
			}
		}
	}
	return Status;
}

std::string Game::Right_Mouse_Pressed(Mine_Button& ref_Button)
{
	std::string Status = "Right_Mouse_Pressed() Completed!";
	if (ref_Button.is_Active())
	{
		ref_Button.Set_Flagged();
	}
	return Status;
}

std::string Game::Reset_Game()
{
	Buttons.clear();
	score = 0;
	max_score = 0;
	Game_Lost = false;
	Window->setVisible(false);
	return "Rese_Game() Completed!";
}