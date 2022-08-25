#include "Projectile.h"
#include <math.h>
#include "Terrain.h"
const float pi = std::acos(-1);
constexpr float frame = 1.f / 60.f;

Projectile::Projectile()
{
}


Projectile::Projectile(sf::Vector2f p, float angle, float sp, float explr)
{
	speed = sp;
	shape = new sf::CircleShape();
	shape->setFillColor(sf::Color::Magenta);
	proj_rad = 4;
	static_cast<sf::CircleShape*>(shape)->setRadius(proj_rad);
	static_cast<sf::CircleShape*>(shape)->setOrigin(proj_rad / 2, proj_rad/2);
	shape->setPosition(p);
	shape->setRotation(angle);
	cur_angle = angle;
	init_angle = angle;
	expl_rad = explr;
	movementVector.x = speed * (cos(cur_angle * pi / 180));
	movementVector.y = speed * (sin(cur_angle * pi / 180));
	//time.restart();

}

Projectile::Projectile(Projectile_type t, sf::Vector2f p, float angle, float sp, float explr, float rad)
{
	type = t;
	if (type == Projectile_type::Laser) {
		speed = sp;
		shape = new sf::RectangleShape();
		shape->setFillColor(sf::Color::Red);
		static_cast<sf::RectangleShape*>(shape)->setSize(sf::Vector2f(1000,rad));
		static_cast<sf::RectangleShape*>(shape)->setOrigin(0, proj_rad/2 );//not working properly for some reason
		shape->setPosition(p);
		shape->setRotation(angle);
		cur_angle = angle;
		init_angle = angle;
		init_pos = p;
		proj_rad = rad;
		time.restart();
	}
	else {
		speed = sp;
		shape = new sf::CircleShape();
		shape->setFillColor(sf::Color::Magenta);
		proj_rad = rad;
		static_cast<sf::CircleShape*>(shape)->setRadius(proj_rad);
		static_cast<sf::CircleShape*>(shape)->setOrigin(proj_rad / 2, proj_rad / 2);
		shape->setPosition(p);
		shape->setRotation(angle);
		cur_angle = angle;
		init_angle = angle;
		init_pos = p;
		expl_rad = explr;
		movementVector.x = speed * (cos(cur_angle * pi / 180));
		movementVector.y = speed * (sin(cur_angle * pi / 180));
	}

	//test
	//time.restart();
}

Projectile::~Projectile()
{
	delete shape;
}

void Projectile::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(*shape, states);
}

void Projectile::moveLazor(Terrain & terrain) {
	if (time.getElapsedTime().asSeconds() >= 0.01f) {
		destroyed = true;
		return;
	}
	//terrain.rayDestroy(init_pos, cur_angle, proj_rad);


}

void Projectile::move(Terrain & terrain) {
	if (destroyed) return;
	if (type == Projectile_type::Laser) {
		Projectile::moveLazor(terrain);
		return;
	}
	sf::Vector2f npos;
	sf::Vector2f cpos = shape->getPosition();

	npos.x = cpos.x + movementVector.x;
	npos.y = cpos.y + movementVector.y;
	
	//npos.x = cpos.x + speed * (cos(cur_angle * pi / 180));
	//npos.y = cpos.y + speed * (sin(cur_angle * pi / 180));
	shape->setPosition(npos);
	movementVector.y += drop_rate * frame;
	//cur_angle = atan2(movementVector.y, movementVector.x) * 180/pi;
	//shape->setRotation(cur_angle);
	/*if (cpos.x >aa < 800) {
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
	//float step = 1;
	// 
	// 	   USEFUL CODE - BE CAREFUL!!!
	for(int i = 0; i<=(speed+ static_cast<sf::CircleShape*>(shape)->getRadius())&& !destroyed;i++) {
		Range* cur = terrain[cpos.x];
		while (cur) {
			if (600 - (cpos.y) >= cur->min && 600 - (cpos.y) <= cur->max) {
				terrain.destroy_circle(sf::Vector2i(npos.x, npos.y), expl_rad);
				destroyed = true;
				speed = 0;
				shape->setFillColor(sf::Color::Transparent);
				break;
			}
			cur = cur->next;
		}
		cpos.x = cpos.x + 1.0 * (cos(cur_angle * pi / 180.0f));
		cpos.y = cpos.y + 1.0 * (sin(cur_angle * pi / 180.0f));
	}

	//use this and make it more accurate
	/*float cx = cpos.x - (proj_rad / 2) * (cos(cur_angle * pi / 180.0f));
	float cy = cpos.y - (proj_rad / 2) * (sin(cur_angle * pi / 180.0f));
	
	int step = movementVector.x > 0 ? 1 : -1;
	for (int i = cx; i < 800 && i>0 && !destroyed; i += step) {
		Range* cur = terrain[cx];
		while (cur) {

			cur = cur->next;
		}
	}*/

	if (npos.x < 0 || npos.x >= 800 || npos.y <-2000 || npos.y>=600 ) {
		destroyed = true;
	}
	//cur_angle -= 10;
}