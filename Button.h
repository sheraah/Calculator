#pragma once
#include<SFML/Graphics.hpp>

class Button
{
private:
	sf::Color buttonsDefaultColor;
	unsigned int DefaultChSize;

public:
	sf::Vector2f DefalutButtonSize;
	Button(float Text_pos_x, float Text_pos_y, unsigned int chSize, sf::Color Textcolor, float OutlineThickness, sf::Color OutlineColor, std::string Text, sf::Font& font, sf::Color buttonColor);
	Button();
	~Button() { };

	sf::RectangleShape button;
	sf::Text buttonText;

	bool ifCollideWithButton(sf::Vector2i* MousePos, sf::Color newColor);
	void render(sf::RenderWindow* window);
};

