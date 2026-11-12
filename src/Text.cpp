#include "Text.h"
Text::Text()
{

}
Text::~Text()
{
	if (text != nullptr)
	{
		delete text;
		text = nullptr;
	}
}
bool Text::initialiseText(std::string text_string)
{
	if (!font.openFromFile("../Data/Fonts/OpenSans-Bold.ttf"))
	{  
		std::cout << "Font no load" << std::endl;
		return false;
	}

	text = new sf::Text(font);
	text->setFont(font);
	text->setString(text_string);
	text->setCharacterSize(50);
	return true;

}

sf::Text* Text::getText()
{
	return text;
}