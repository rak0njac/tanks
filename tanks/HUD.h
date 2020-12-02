#pragma once
#include <SFML/Graphics.hpp>
class HUD: public sf::Drawable
{
public:
	HUD(const std::string &filename);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	~HUD();


private:
	sf::Font font;
	sf::Text fireText;
	sf::RectangleShape fireRect;
	sf::Text angleText;
	sf::RectangleShape angleRect;
	sf::Text moveText;
	sf::RectangleShape moveRect;
	sf::Text weaponText;
	sf::RectangleShape weaponRect;
};

