#include "Main_Menu.h"
#include <iostream>
Main_Menu::Main_Menu()
{
    Menu_Window = std::move(std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(300, 300), "Saper")));


    std::vector<std::string> settup_helper{ "Menu","Play","Quit","Easy","Normal","Hard" };

    std::shared_ptr<sf::Image> filling_Map(new sf::Image);
    std::shared_ptr<sf::Texture> filling_Texture_Map(new sf::Texture);
    for (auto it_settup_helper = settup_helper.begin(); it_settup_helper != settup_helper.end(); ++it_settup_helper)
    {
        filling_Map->loadFromFile("Sprite//" + *it_settup_helper + ".png");
        Image_Map[*it_settup_helper] = filling_Map;
        filling_Texture_Map->loadFromImage(*Image_Map[*it_settup_helper]);
        Menu_Texture[*it_settup_helper] = filling_Texture_Map;
        filling_Map = std::make_shared<sf::Image>();
        filling_Texture_Map = std::make_shared<sf::Texture>();
    }

    Menu_Sprite = std::make_shared<sf::Sprite>();
    Menu_Sprite->setTexture(*Menu_Texture["Menu"]);
    
    {
        std::shared_ptr<sf::Sprite> Helper_Sprite = std::make_shared<sf::Sprite>();
        std::shared_ptr<sf::Texture> Temp_Texture_Pointer = Menu_Texture["Play"];
        Helper_Sprite->setTexture(*Temp_Texture_Pointer);
        Helper_Sprite->move((Menu_Window->getSize().x / 2) - (Temp_Texture_Pointer->getSize().x / 2), (Menu_Window->getSize().y / 3) - (Temp_Texture_Pointer->getSize().y / 2));
        Outer_Buttons.push_back(Helper_Sprite);

        Helper_Sprite = std::make_shared<sf::Sprite>();
        Temp_Texture_Pointer = Menu_Texture["Quit"];
        Helper_Sprite->setTexture(*Temp_Texture_Pointer);
        Helper_Sprite->move((Menu_Window->getSize().x / 2) - (Temp_Texture_Pointer->getSize().x / 2), ((Menu_Window->getSize().y / 3) - (Temp_Texture_Pointer->getSize().y / 2)) + (Temp_Texture_Pointer->getSize().y * 2));
        Outer_Buttons.push_back(Helper_Sprite);

        Helper_Sprite = std::make_shared<sf::Sprite>();
        Temp_Texture_Pointer = Menu_Texture["Easy"];
        Helper_Sprite->setTexture(*Temp_Texture_Pointer);
        Helper_Sprite->move((Menu_Window->getSize().x / 2) - (Temp_Texture_Pointer->getSize().x / 2), ((Menu_Window->getSize().y / 2) - (Temp_Texture_Pointer->getSize().y / 2) - 50) + 50 * Difficulty_Buttons.size());
        Difficulty_Buttons.push_back(Helper_Sprite);

        Helper_Sprite = std::make_shared<sf::Sprite>();
        Temp_Texture_Pointer = Menu_Texture["Normal"];
        Helper_Sprite->setTexture(*Temp_Texture_Pointer);
        Helper_Sprite->move((Menu_Window->getSize().x / 2) - (Temp_Texture_Pointer->getSize().x / 2), ((Menu_Window->getSize().y / 2) - (Temp_Texture_Pointer->getSize().y / 2) - 50) + 50 * Difficulty_Buttons.size());
        Difficulty_Buttons.push_back(Helper_Sprite);

        Helper_Sprite = std::make_shared<sf::Sprite>();
        Temp_Texture_Pointer = Menu_Texture["Hard"];
        Helper_Sprite->setTexture(*Temp_Texture_Pointer);
        Helper_Sprite->move((Menu_Window->getSize().x / 2) - (Temp_Texture_Pointer->getSize().x / 2), ((Menu_Window->getSize().y / 2) - (Temp_Texture_Pointer->getSize().y / 2) - 50) + 50 * Difficulty_Buttons.size());
        Difficulty_Buttons.push_back(Helper_Sprite);
    }

}


void Main_Menu::Begin()
{
    Mine_Sweeper = std::make_unique<Game>();


    while (Menu_Window->isOpen())
    {
        sf::Event event;
        while (Menu_Window->pollEvent(event))
        {
            std::cout << "Test" << std::endl;
            if (event.type == sf::Event::Closed)
            {
                Menu_Window->close();
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                if(Outer_Buttons[0]->getGlobalBounds().contains(Menu_Window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
                {
                    Play_Clicked();
                }
                else if (Outer_Buttons[1]->getGlobalBounds().contains(Menu_Window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
                {
                    Menu_Window->close();
                }
            
            }
        }

        Menu_Window->clear();
        Menu_Window->draw(*Menu_Sprite);
        for (auto it : Outer_Buttons)
        {
            Menu_Window->draw(*it);
        }
        Menu_Window->display();
    }

}


int Main_Menu::Play_Clicked()
{
    while (Menu_Window->isOpen())
    {
        sf::Event event;
        while (Menu_Window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                Menu_Window->close();
            }
            else if(event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    for (auto it = Difficulty_Buttons.begin(); it != Difficulty_Buttons.end(); ++it)
                    {
                        if ((*it)->getGlobalBounds().contains(Menu_Window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
                        {
                            Menu_Window->setVisible(false);
                            Mine_Sweeper->Setting_up(Game::Difficulty(it - Difficulty_Buttons.begin()));
                            Mine_Sweeper->Begin();
                            Mine_Sweeper->Reset_Game();
                            Menu_Window->setVisible(true);
                            return 1;
                        }
                    }   
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    return 1;
                }
            }
        }

        Menu_Window->clear();

        Menu_Window->draw(*Menu_Sprite);
        for (auto it : Difficulty_Buttons)
        {
            Menu_Window->draw((*it));
        }
        Menu_Window->display();
    }
    return 0;
}