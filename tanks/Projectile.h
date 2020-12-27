#pragma once
#include <SFML/Graphics.hpp>
#include "Terrain.h"

enum class Projectile_type {
	Rect,
	Circ
};

class Projectile: public sf::Drawable
{
public:
	Projectile();
	Projectile(sf::Vector2f p, float angle);
	Projectile(sf::Vector2f p, float angle, float sp);
	Projectile(sf::Vector2f p, float angle, float sp, float explr);
	~Projectile();
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
	void move(Terrain & terrain);
	void update();

	sf::Vector2f movementVector;
	float speed;
	sf::Shape *shape;
	float cur_angle;
	float gravity_factor;
	bool destroyed = false;
	float expl_rad;
	float proj_rad;
};

