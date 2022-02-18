#include "Button.h"

Button::Button(float Text_pos_x, float Text_pos_y, unsigned int chSize, sf::Color Textcolor, float OutlineThickness, sf::Color OutlineColor, std::string Text, sf::Font& font, sf::Color buttonColor)
	:buttonsDefaultColor(buttonColor), DefaultChSize(chSize)
{
	buttonText.setPosition(Text_pos_x, Text_pos_y);
	buttonText.setCharacterSize(chSize);
	buttonText.setString(Text);
	buttonText.setFont(font);
	buttonText.setFillColor(Textcolor);
	buttonText.setOutlineThickness(OutlineThickness);
	buttonText.setOutlineColor(OutlineColor);

	button.setFillColor(buttonColor);
	button.setPosition(buttonText.getPosition().x, buttonText.getPosition().y + buttonText.getGlobalBounds().height / 10);
	button.setSize({ buttonText.getGlobalBounds().width, buttonText.getGlobalBounds().height });

	DefalutButtonSize = button.getSize();
}

Button::Button()
{
}

bool Button::ifCollideWithButton(sf::Vector2i* MousePos, sf::Color newColor)
{
	if (MousePos->x >= button.getPosition().x && MousePos->x <= button.getPosition().x + button.getGlobalBounds().width &&
		MousePos->y >= button.getPosition().y && MousePos->y <= button.getPosition().y + button.getGlobalBounds().height)
	{
		button.setFillColor(newColor);
		buttonText.setCharacterSize(DefaultChSize + DefaultChSize / 10);
		button.setSize({ DefalutButtonSize.x + DefalutButtonSize.x / 10, DefalutButtonSize.y + DefalutButtonSize.y / 10 });
		return true;
	}

	buttonText.setCharacterSize(DefaultChSize);
	button.setFillColor(buttonsDefaultColor);
	button.setSize(DefalutButtonSize);
	return false;
}

void Button::render(sf::RenderWindow* window)
{
	window->draw(button);
	window->draw(buttonText);
}
