#pragma once
#include <SFML/Graphics.hpp>
#include "Terrain.h"
const float fallSpeed = 2.f;
const float moveSpeed = 1.f;
class Player: public sf::Drawable
{
public:
	Player();
	~Player();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void move(const Terrain& terrain, sf::Vector2f mv);
	bool movingLeft = false;
	bool movingRight = false;
private:
	sf::RectangleShape body;
	sf::RectangleShape tube;
	sf::CircleShape collisionCircle;
	sf::Vector2f pos;
	sf::Vector2f movementVec;
	float rotation = 0;
	
	bool falling = true;

};

