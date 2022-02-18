#include<SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include "ShapeWithTex.h"
#include "Button.h"

sf::RenderWindow window(sf::VideoMode(600, 800), "Calculator", sf::Style::Close);

sf::Vector2f operator*(const sf::Vector2f& vec, double other)
{
	return sf::Vector2f(vec.x * other, vec.y * other);
}

sf::Font font;

namespace Math
{
	std::string AllMathSymbols = "*/:x^%^-+";

	bool isInteger(double Val)
	{
		return int(Val) == Val;
	}

	bool isNumber(char c)
	{
		return (c >= '0' && c <= '9');
	}

	double MathTask(double Dval1, double Dval2, char ope)
	{
		if (ope == '*' || ope == 'x') return Dval1 * Dval2;
		if (ope == ':' || ope == '/') return Dval1 / Dval2;
		if (ope == '+') return Dval1 + Dval2;
		if (ope == '-') return Dval1 - Dval2;
		if (ope == '^') return pow(Dval1, Dval2);
		if (ope == '%') return int(Dval1) % int(Dval2);
	}

	double stringToNumber(std::string Num)
	{
		double result = 0;
		std::stringstream ss;
		ss << Num;
		ss >> result;

		return result;
	}

	double stringToTask(std::string task)
	{
		std::vector<double> Nums;
		std::string operators;
		{
			std::string stringNum = "";

			for (size_t i = 0; i < task.length(); i++)
				if (task[i] != ' ')
				{
					if (isNumber(task[i]) || task[i] == '.')
						stringNum.push_back(task[i]);

					else if (task[i] == '+' || task[i] == '-' || task[i] == '/' || task[i] == ':' || task[i] == '*' ||
						task[i] == 'x' || task[i] == '^' || task[i] == '%')
					{
						operators.push_back(task[i]);
						if (stringNum != "")
						{
							Nums.push_back(stringToNumber(stringNum));
							stringNum = "";
						}
					}


				}

			Nums.push_back(stringToNumber(stringNum));
		}

		do
		{
			std::string MathSymbols = "";

			for (size_t i = 0; i < operators.length(); i++)
			{
				if (operators[i] == '^' || operators[i] == '%') { MathSymbols = "^%"; break; }
				if (operators[i] == '*' || operators[i] == '/' || operators[i] == ':' || operators[i] == 'x') MathSymbols = "*/:x^";
			}

			if (MathSymbols == "") MathSymbols = "+-";

			for (size_t i = 0; i < operators.size(); i++)
			{
				for (size_t j = 0; j < MathSymbols.length(); j++)
					if (operators[i] == MathSymbols[j])
					{
						double Val = MathTask(Nums[i], Nums[i + 1], operators[i]);
						operators.erase(operators.begin() + i);
						Nums.erase(Nums.begin() + i);
						Nums.erase(Nums.begin() + i);
						Nums.insert(Nums.begin() + i, Val);
						break;
					}
			}

		} while (!operators.empty());


		return Nums[0];
	}
}

namespace button
{

	constexpr size_t ButtonCount = 20;
	char Buttonchars[ButtonCount] = { 'C', '<', '.', '^', '%', ':', '7', '8', '9', '*', '4', '5', '6', '-', '1', '2', '3', '+', '0', '=' };
	const sf::Color MouseHoverColor = sf::Color::Color(100, 100, 100, 50);
	sf::RectangleShape HoverShape;

	const sf::Vector2f ButtonSize = { 150.f, 100.f };

	std::vector<sf::RectangleShape> Buttons;

	std::string CurrentTask = "";
	sf::Text CurrentTaskText;
	std::string PrevTask = "";

	bool MouseHeld = false;
	int ChSize = 45;

	void init()
	{
		CurrentTaskText.setCharacterSize(30);
		CurrentTaskText.setFont(font);
		CurrentTaskText.setString(CurrentTask);
		int yTimesValue = 2;
		bool Corner = true;

		for (size_t i = 0; i < ButtonCount; i++)
		{
			int TimesValue = 1;
			sf::Color ButtonColor = sf::Color::Color(16, 16, 16);

			if (Buttonchars[i] == 'C' || Buttonchars[i] == '<')
			{
				TimesValue = 2;
				ButtonColor = sf::Color::Color(25, 25, 25);
			}

			if (Buttonchars[i] == '0') TimesValue = 3;


			Buttons.push_back(sf::RectangleShape({ ButtonSize.x * TimesValue, ButtonSize.y }));
			Buttons[i].setFillColor(ButtonColor);

			if (Corner)
			{
				Buttons[i].setPosition(0.f, yTimesValue * 100.f); Corner = false;
			}
			else
			{
				Buttons[i].setPosition(Buttons[i - 1].getPosition().x + Buttons[i - 1].getSize().x, yTimesValue * 100.f);
			}
			Buttons[i].setOutlineThickness(3.f);
			Buttons[i].setOutlineColor(sf::Color::Color(32, 32, 32));


			if (i >= 0 && Buttons[i].getPosition().x + Buttons[i].getSize().x >= window.getSize().x)
			{
				yTimesValue++;
				Corner = true;
			}

		}
	}

	void render()
	{
		for (size_t i = 0; i < Buttons.size(); i++)
		{
			window.draw(Buttons[i]);
			sf::Text ButtonChar;
			ButtonChar.setCharacterSize(Buttons[i].getSize().x / 6);
			if (Buttonchars[i] == '0') ButtonChar.setCharacterSize(Buttons[i].getSize().x / 12);
			ButtonChar.setFont(font);
			ButtonChar.setString(Buttonchars[i]);
			ButtonChar.setPosition(Buttons[i].getPosition().x + Buttons[i].getGlobalBounds().width / 2 - ButtonChar.getGlobalBounds().width,
				Buttons[i].getPosition().y + Buttons[i].getGlobalBounds().height / 2 - ButtonChar.getGlobalBounds().height);

			window.draw(ButtonChar);
		}

		window.draw(HoverShape);



		CurrentTaskText.setPosition(window.getSize().x - CurrentTaskText.getGlobalBounds().width - 10, Buttons[0].getPosition().y - ButtonSize.y / 1.5);

		window.draw(CurrentTaskText);

		sf::Text PrevText;
		PrevText.setFont(font);
		PrevText.setCharacterSize(ChSize - 10);
		PrevText.setString(PrevTask);
		PrevText.setOutlineThickness(2.f);
		PrevText.setOutlineColor(sf::Color::Black);
		PrevText.setPosition(window.getSize().x - PrevText.getGlobalBounds().width - 10, CurrentTaskText.getPosition().y - ButtonSize.y);

		window.draw(PrevText);
	}

	void update()
	{
		bool isHovered = false;

		for (size_t i = 0; i < Buttons.size(); i++)
		{
			if (Buttons[i].getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
			{
				isHovered = true;
				HoverShape.setSize({ Buttons[i].getSize() });
				HoverShape.setFillColor(MouseHoverColor);
				HoverShape.setPosition(Buttons[i].getPosition());

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !MouseHeld)
				{
					MouseHeld = true;

					if (Buttonchars[i] == 'C') CurrentTask = "";

					else if (Buttonchars[i] == '<')
					{
						if (CurrentTask.length() <= 0) break;
						std::string newTask = "";
						for (size_t i = 0; i < CurrentTask.length() - 1; i++)
							newTask += CurrentTask[i];

						CurrentTask = newTask;
					}

					else if (Buttonchars[i] == '=')
					{
						PrevTask = CurrentTask;

						double result = Math::stringToTask(CurrentTask);

						if (Math::isInteger(result)) CurrentTask = std::to_string(int(result));
						else CurrentTask = std::to_string(result);
					}

					else
					{
						bool canAddChar = true;

						for (const auto& M : Math::AllMathSymbols)
						{
							if (Buttonchars[i] == M)
							{
								if (CurrentTask.length() <= 0)
								{
									canAddChar = false;
									break;
								}
								for (const auto& K : Math::AllMathSymbols)
								{
									if (CurrentTask[CurrentTask.length() - 1] == K)
										canAddChar = false;
								}
							}
						}

						if (Buttonchars[i] == '.')
							if (CurrentTask.length() <= 0) canAddChar = false;


						if (canAddChar)
							CurrentTask += Buttonchars[i];
					}


					CurrentTaskText.setString(CurrentTask);
				}

				if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) MouseHeld = false;
			}
		}

		if (!isHovered) HoverShape.setPosition(window.getSize().x, window.getSize().y);


	}
}

int main()
{
	window.setFramerateLimit(60);
	std::string s = "Small.ttf";
	font.loadFromFile(s);

	button::init();

	while (window.isOpen())
	{
		sf::Event evnt;
		while (window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}


		button::update();


		window.clear(sf::Color(32, 32, 32));

		button::render();

		window.display();
	}

}