#pragma once
#ifndef BUTTON_H_
#define BUTTON_H_
#include <SFML/Graphics.hpp>
#include <memory>

class Mine_Button
{
private:
	sf::Sprite SButton;
	std::shared_ptr<sf::Image> After_click_Image;
	std::shared_ptr<sf::Image> Active_Image;
	std::shared_ptr<sf::Image> Flaged;
	sf::Texture TButton;
	int mine;
	bool is_active;
	bool is_mine;
	bool is_flaged;
public:
	Mine_Button() { 
		mine = 0;
		is_mine = false;
		is_active = true;
		is_flaged = false;
	}

	void Set_Active_Texture(std::shared_ptr<sf::Image> Im) {
		Active_Image = Im;
		TButton.loadFromImage(*Active_Image);
		SButton.setTexture(TButton);
	}

	void Set_After_Click_Image(std::shared_ptr<sf::Image> Im) {
		After_click_Image = Im;
	}

	void Set_Flagged_Image(std::shared_ptr<sf::Image> Im) {
		Flaged = Im;
	}

	void Set_Flagged()
	{
		if (!is_flaged)
		{
			TButton.loadFromImage(*Flaged);
			SButton.setTexture(TButton);
			is_flaged = true;
		}
		else
		{
			TButton.loadFromImage(*Active_Image);
			SButton.setTexture(TButton);
			is_flaged = false;
		}
	}

	void Set_mine(int value) {
		mine = value;
	}

	int Clicked()
	{
		TButton.loadFromImage(*After_click_Image);
		SButton.setTexture(TButton);
		is_active = false;
		if (mine == -1)
			return mine;
		else
			return 1;
	}

	sf::Sprite& GetSprite() {
		return SButton;
	};
	const int GetMine()	
	{
		return mine;
	};
	bool is_Active() {
		return is_active;
	}
	bool is_Mine() {
		if (mine == -1)
			return true;
		else
			return false;
	}

	bool is_Flagged()
	{
		return is_flaged;
	}
};
#endif // !BUTTON_H_
