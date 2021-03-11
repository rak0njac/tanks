#pragma once
#include <SFML/Graphics.hpp>
#include "Terrain.h"

enum class Projectile_type {
	Rect,
	Circ,
	LAZOR
};

class Projectile: public sf::Drawable
{
public:
	Projectile();
	Projectile(sf::Vector2f p, float angle);
	Projectile(sf::Vector2f p, float angle, float sp);
	Projectile(sf::Vector2f p, float angle, float sp, float explr);
	Projectile(Projectile_type t, sf::Vector2f p, float angle, float sp, float explr, float rad);
	Projectile(sf::Vector2f p, float angle, float sp, float explr, float rad);
	~Projectile();
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
	void moveLazor(Terrain & terrain);
	void move(Terrain & terrain);
	void update();

	sf::Vector2f movementVector;
	sf::Vector2f init_pos;
	sf::Clock time;
	float speed;
	sf::Shape *shape;
	Projectile_type type = Projectile_type::Circ;
	float cur_angle;
	bool destroyed = false;
	float expl_rad;
	float proj_rad;
	float drop_rate = 0;
	float drop_max = 0;
	float init_angle;
};

