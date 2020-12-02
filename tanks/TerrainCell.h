#pragma once
#include <SFML/Graphics.hpp>

class TerrainCell
{
public:
	TerrainCell();
	~TerrainCell();

	sf::Vector2f topLeft;
	sf::Vector2f topRight;
	sf::Vector2f botLeft;
	sf::Vector2f botRight;
	

	sf::ConvexShape shape;
};

