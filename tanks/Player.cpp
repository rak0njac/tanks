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
	collider.setFillColor(sf::Color::Blue);
	collider.setPointCount(5);
	collider.setRadius(5);
	collider.setPosition(100, 10);
	collider.setOrigin(collider.getLocalBounds().width / 2, collider.getLocalBounds().height);

	tube.setFillColor(sf::Color::Green);
	tube.setSize(sf::Vector2f(15, 3));
	//tube.setPosition(collider.getPosition());
	tube.setOrigin(sf::Vector2f(1.5, 1.5));

	body.setSize(sf::Vector2f(20, 10));
	body.setFillColor(sf::Color::Red);
	//body.setPosition(collider.getPosition());
	body.setOrigin(body.getLocalBounds().width / 2, body.getLocalBounds().height - 2);
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
	target.draw(body, states);
	target.draw(tube, states);
}

void Player::logic(const Terrain& terrain, sf::Vector2f mv, const sf::Vector2f& mousepos, GameResourceManager& grm)
{
	if (firing) shoot(grm.projectiles);
	move(terrain);
}

void Player::move_tube(const sf::Vector2f& mousepos) {
	float tube_angle = atan2(mousepos.y - tube.getPosition().y, mousepos.x - tube.getPosition().x);
	tube_angle *= 180 / pi;
	tube.setRotation(tube_angle);
}

void Player::move(const Terrain& terrain)
{
	// EDGE CASES TODO: refactor
	if (collider.getPosition().x < 10) {
		direction = 0;
		collider.setPosition(10, collider.getPosition().y);
	}
	if (collider.getPosition().x > 790) {
		direction = 0;
		collider.setPosition(790, collider.getPosition().y);
	}
	if (collider.getPosition().y < 0) {
		collider.setPosition(collider.getPosition().x, 0);
	}
	if (collider.getPosition().y > 600) {
		collider.setPosition(collider.getPosition().x, 600);
	}

	// COLLISION POINTS
	// sf::Circle "collider" has 5 points, we need the current horizontal position of the bottommost 
	// two points in order to perform a perpendicular calculation on that line
	int first_collision_point = collider.getTransform().transformPoint(collider.getPoint(3)).x;
	int second_collision_point = collider.getTransform().transformPoint(collider.getPoint(2)).x;

	//TODO make ranges obsolete and figure out a better way
	const Range* prev_range = &terrain.ranges[first_collision_point];			//the range of vertical pixels from terrain at the location of the first collision point
	const Range* current_range = &terrain.ranges[collider.getPosition().x];		//the range between the first and second collision point (used for moving, not important for collision)
	const Range* next_range = &terrain.ranges[second_collision_point];

	if (collider.getPosition().y < (600 - current_range->max) - 3) { // if (collision detected); TODO: 3 is the best magicnum out of them all....
		collider.move(0, const_falling_speed);
		tube.move(0, const_falling_speed);
	}

	else if (direction != 0) {
		float x1 = first_collision_point;
		float x2 = second_collision_point;
		float y1 = 600 - prev_range->max;
		float y2 = 600 - next_range->max;

		float angle = atan2(y2 - y1, x2 - x1) * 180 / pi;	// y2 - y1 and x2 - x1 is the perpendicular line to the line formed out of terrain pixels exactly below our horizontal collision points. And then we just atan to get the angle.

		//steep hill edge cases
		if (angle > 60.0f && direction == -1) return;	
		if (angle < -60.0f && direction == 1) return;

		float new_pos_x = collider.getPosition().x + direction;		// every horizontal move is always 1px
		current_range = &terrain.ranges[collider.getPosition().x];	// get the next vertical pixel range from terrain since we moved horizontally by 1px
		float new_pos_y = 600 - current_range->max;					// set our new vertical position to the range's topmost pixel

		collider.setPosition(new_pos_x, new_pos_y);
		collider.setRotation(angle);
	}

	tube.setPosition(collider.getPosition().x, collider.getPosition().y - 5);	//TODO remove magicnum
	tube.setRotation(-45);		//TODO

	body.setPosition(collider.getPosition());
	body.setRotation(collider.getRotation());
}