#include "HUD.h"
#include <iostream>


HUD::HUD(const std::string &filename)
{
	//if (!font.loadFromFile(filename))
	//{
	//	std::cout << "can't load font" << std::endl;
	//}
	

	fireText.setFont(font);
	moveText.setFont(font);
	angleText.setFont(font);
	weaponText.setFont(font);

	fireText.setString("Fire");
	moveText.setString("Move");
	angleText.setString("Angle");
	weaponText.setString("Weapon");

	fireRect.setSize(sf::Vector2f(90.f, 30.f));
	moveRect.setSize(sf::Vector2f(90.f, 30.f));
	angleRect.setSize(sf::Vector2f(90.f, 30.f));
	weaponRect.setSize(sf::Vector2f(90.f, 30.f));

	fireRect.setPosition(sf::Vector2f(420.f, 500.f));
	moveRect.setPosition(sf::Vector2f(100.f, 500.f));
	angleRect.setPosition(sf::Vector2f(530.f, 500.f));
	weaponRect.setPosition(sf::Vector2f(210.f, 500.f));

	fireRect.setOutlineColor(sf::Color::Red);
	fireRect.setFillColor(sf::Color::Transparent);
	fireRect.setOutlineThickness(3.f);

	moveRect.setOutlineColor(sf::Color::Red);
	moveRect.setFillColor(sf::Color::Transparent);
	moveRect.setOutlineThickness(3.f);

	angleRect.setOutlineColor(sf::Color::Red);
	angleRect.setFillColor(sf::Color::Transparent);
	angleRect.setOutlineThickness(3.f);

	weaponRect.setOutlineColor(sf::Color::Red);
	weaponRect.setFillColor(sf::Color::Transparent);
	weaponRect.setOutlineThickness(3.f);

	fireText.setPosition(440, 500.f);
	moveText.setPosition(110, 500.f);
	angleText.setPosition(550, 500.f);
	weaponText.setPosition(220, 500.f);

	fireText.setFillColor(sf::Color::Red);
	fireText.setCharacterSize(20);
	moveText.setCharacterSize(20);
	angleText.setCharacterSize(20);
	weaponText.setCharacterSize(20);

}

void HUD::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(fireRect, states);
	target.draw(moveRect, states);
	target.draw(angleRect, states);
	target.draw(weaponRect, states);

	target.draw(fireText);
	target.draw(moveText);
	target.draw(angleText);
	target.draw(weaponText);
}
HUD::~HUD()
{
}
