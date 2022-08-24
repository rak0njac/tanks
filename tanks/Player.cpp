#include "Player.h"
#include <stdio.h>
#include <cmath>
#include <iostream>

const float pi = std::acos(-1);

sf::Vector2f closestPointOnLine(const sf::Vector2f& l1, const sf::Vector2f& l2, const sf::Vector2f& p) {
	float a1 = l2.y - l1.y;
	float b1 = l1.x - l2.x;
	double c1 = (l2.y - l1.y) * l1.x + (l1.x - l2.x) * l1.y;
	double c2 = -b1 * p.x + a1 * p.y;
	double det = (a1 * a1 - (-b1 * b1));
	sf::Vector2f c;
	if (det != 0) {
		c.x = (float)((a1 * c1 - b1 * c2) / det);
		c.y = (float)((a1 * c2 - (-b1 * c1)) / det);
	}
	else {
		c.x = p.x;
		c.y = p.y;
	}
	return c;
}



Player::Player()
{
	body.setFillColor(sf::Color::Blue);
	body.setSize(sf::Vector2f(20, 10));
	body.setPosition(100, 10);
	body.setOrigin(body.getLocalBounds().width / 2, body.getLocalBounds().height);

	tube.setFillColor(sf::Color::Green);
	tube.setSize(sf::Vector2f(15, 3));
	tube.setPosition(body.getPosition().x, body.getPosition().y - 5);
	tube.setOrigin(sf::Vector2f(0, 1.5));
}


Player::~Player()
{
}

void Player::shoot(Weapon& weapon) {}

void Player::shoot(std::list <Projectile*>& projectiles) {
	if (weapons.size() <= 0) return;
	weapons[currentWeapon].fire(tube.getPosition(), tube.getRotation(), projectiles);
}

void Player::add_weapon(Weapon wep)
{
	weapons.push_back(wep);
}

void Player::change_weapon(int i)
{
	currentWeapon += i;
	if (currentWeapon < 0) currentWeapon = weapons.size() - 1;
	else currentWeapon %= weapons.size();
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(tube, states);
	target.draw(body, states);
}

void Player::logic(const Terrain& terrain, sf::Vector2f mv, const sf::Vector2f& mousepos, GameResourceManager& grm)
{
	if (firing) shoot(grm.projectiles);
	move_tube(mousepos);
	move(terrain);
}

void Player::move_tube(const sf::Vector2f& mousepos) {
	float tube_angle = atan2(mousepos.y - tube.getPosition().y, mousepos.x - tube.getPosition().x);
	tube_angle *= 180 / pi;
	tube.setRotation(tube_angle);
}

void Player::move(const Terrain& terrain)
{
	if (body.getPosition().x > 799) {
		body.setPosition(799, body.getPosition().y);
	}
	if (body.getPosition().y < 0) {
		body.setPosition(body.getPosition().x, 0);
	}
	if (body.getPosition().y > 450) {
		body.setPosition(body.getPosition().x, 450);
	}

	const Range* prev_range = &terrain.ranges[body.getPosition().x - 1];
	const Range* current_range = &terrain.ranges[body.getPosition().x];
	const Range* next_range = &terrain.ranges[body.getPosition().x + 1];

	if (body.getPosition().y < 600 - current_range->max) {
		falling = true;
		body.move(0, const_falling_speed);
		tube.move(0, const_falling_speed);
	}
	else falling = false;

	if (!falling && direction != 0) {
		float x1 = body.getPosition().x - 1;
		float x2 = body.getPosition().x + 1;
		float y1 = 600 - prev_range->max;
		float y2 = 600 - next_range->max;

		float angle = atan2(y2 - y1, x2 - x1) * 180 / pi;

		std::cout << prev_range->max << std::endl;
		std::cout << next_range->max << std::endl;
		std::cout << std::endl;

		body.setPosition(body.getPosition().x + direction, 600 - current_range->max);
		tube.setPosition(tube.getPosition().x + direction, 600 - current_range->max - 5);
		body.setRotation(angle);
		}
}