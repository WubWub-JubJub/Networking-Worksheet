#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class Text
{
public:
	Text();
	~Text();

	bool initialiseText(std::string text_string);
	sf::Text* getText();

private:
	sf::Text* text = nullptr;
	sf::Font font;
};
