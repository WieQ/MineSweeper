#ifndef MAIN_MENU_H_
#define MAIN_MENU_H_
#include "Game.h"

class Main_Menu
{
private:
	std::unique_ptr<sf::RenderWindow> Menu_Window;
	std::unique_ptr<Game> Mine_Sweeper;
	std::unordered_map<std::string, std::shared_ptr<sf::Image> > Image_Map;
	std::shared_ptr<sf::Sprite> Menu_Sprite;
	std::vector<std::shared_ptr<sf::Sprite> > Outer_Buttons;
	std::vector<std::shared_ptr<sf::Sprite> > Difficulty_Buttons;
	std::unordered_map<std::string, std::shared_ptr<sf::Texture> > Menu_Texture;
	
	int Play_Clicked();
public:
	Main_Menu();
	void Begin();
	
};
#endif // !Main_Menu

