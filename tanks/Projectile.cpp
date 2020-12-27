#include "Projectile.h"
#include <math.h>
#include "Terrain.h"
const float pi = std::acos(-1);


Projectile::Projectile()
{
}


Projectile::Projectile(sf::Vector2f p, float angle, float sp, float explr)
{
	speed = sp;
	gravity_factor = 1;
	shape = new sf::CircleShape();
	shape->setFillColor(sf::Color::Magenta);
	proj_rad = 4;
	static_cast<sf::CircleShape*>(shape)->setRadius(proj_rad);
	static_cast<sf::CircleShape*>(shape)->setOrigin(proj_rad/2, proj_rad/2);
	shape->setPosition(p);
	shape->setRotation(angle);
	cur_angle = angle;
	expl_rad = explr;
}

Projectile::~Projectile()
{
	delete shape;
}

void Projectile::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(*shape, states);
}

void Projectile::move(Terrain & terrain) {
	if (destroyed) return;
	sf::Vector2f npos;
	sf::Vector2f cpos = shape->getPosition();
	npos.x = cpos.x + speed * (cos(cur_angle * pi / 180));
	npos.y = cpos.y + speed * (sin(cur_angle * pi / 180));
	shape->setPosition(npos);

	/*if (cpos.x >= 0 && cpos.x < 800) {
		const Range* cur = &terrain.ranges[cpos.x];
		while (cur) {
			if (600 - cpos.y >=  cur->min && 600 - cpos.y <= cur->max) {
				terrain.destroy(sf::Vector2i(npos.x, npos.y), expl_rad);
				destroyed = true;
				speed = 0;
				shape->setFillColor(sf::Color::Transparent);
				//break;
			}
			cur = cur->next;

		}
	}*/
	//int fac = cpos.x > npos.x ? -1 : 1;
	for(int i = 0; i<(speed+ static_cast<sf::CircleShape*>(shape)->getRadius())&& !destroyed;i++) {
		Range* cur = terrain[cpos.x];
		while (cur) {
			if (600 - (cpos.y) >= cur->min && 600 - (cpos.y) <= cur->max) {
				terrain.destroy(sf::Vector2i(npos.x, npos.y), expl_rad);
				destroyed = true;
				speed = 0;
				shape->setFillColor(sf::Color::Transparent);
				break;
			}
			cur = cur->next;
		}

		cpos.x = cpos.x + 1 * (cos(cur_angle * pi / 180));
		cpos.y = cpos.y + 1 * (sin(cur_angle * pi / 180));
	}
	if (npos.x < 0 || npos.x >= 800 || npos.y <0 || npos.y>=600 ) {
		destroyed = true;
	}
	//cur_angle -= 10;
}