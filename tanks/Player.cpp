#include "Player.h"
#include "Game.h"
#include <stdio.h>
#include <cmath>
#include <iostream>

const float pi = std::acos(-1);

sf::Vector2f closestPointOnLine(const sf::Vector2f& l1, const sf::Vector2f& l2, const sf::Vector2f& p) {
	float a1 = l2.y - l1.y;
	float b1 = l1.x - l2.x;
	float c1 = (l2.y - l1.y) * l1.x + (l1.x - l2.x) * l1.y;
	float c2 = -b1 * p.x + a1 * p.y;
	float det = (a1 * a1 - (-b1 * b1));
	sf::Vector2f c;
	if (det != 0) {
		c.x = (a1 * c1 - b1 * c2) / det;
		c.y = (a1 * c2 - (-b1 * c1)) / det;
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

void Player::logic(NewTerrain& terrain, sf::Vector2f mv, const sf::Vector2f& mousepos, GameResourceManager& grm)
{
	//std::cout << mousepos.x << " " << mousepos.y << std::endl;
	if (firing) shoot(grm.projectiles);
	move_tube(mousepos);
	move(terrain);
}

void Player::move_tube(const sf::Vector2f& mousepos) {
	float tube_angle = atan2(mousepos.y - tube.getPosition().y, mousepos.x - tube.getPosition().x);
	tube_angle *= 180 / pi;
	tube.setRotation(tube_angle);
}

void Player::move(NewTerrain& terrain)
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
	if (collider.getPosition().y > 600) {
		collider.setPosition(collider.getPosition().x, 600);
	}

	// COLLISION POINTS
	// sf::Circle "collider" has 5 points, we need the current horizontal position of the bottommost 
	// two points in order to perform a perpendicular calculation on that line
	int first_collision_point = collider.getTransform().transformPoint(collider.getPoint(3)).x;
	int second_collision_point = collider.getTransform().transformPoint(collider.getPoint(2)).x;

	int prev_range_new = terrain.get_terrain()->at(first_collision_point)->top();// terrain.get_top_vertex_position_of_vertical_array_at_width(first_collision_point);
	int current_range_new = terrain.get_terrain()->at(collider.getPosition().x)->top();// terrain.get_top_vertex_position_of_vertical_array_at_width(collider.getPosition().x);
	int next_range_new = terrain.get_terrain()->at(second_collision_point)->top(); // terrain.get_top_vertex_position_of_vertical_array_at_width(second_collision_point);


	if (collider.getPosition().y > current_range_new + collider.getRadius() / 2 + 1) { // if (tank is under the ground)
		return;
	}

	if (collider.getPosition().y < current_range_new - collider.getRadius() / 2 - 1) { // if (collision detected);
		collider.move(0, const_falling_speed);
		tube.move(0, const_falling_speed);
	}

	else {
		float x1 = first_collision_point;
		float x2 = second_collision_point;
		float y1 = prev_range_new;
		float y2 = next_range_new;

		float angle = atan2(y2 - y1, x2 - x1) * 180 / pi;	// y2 - y1 and x2 - x1 is the perpendicular line to the line formed out of terrain pixels exactly below our horizontal collision points. And then we just atan to get the angle.

		//steep hill edge cases
		if (angle > 60.0f && direction == -1) return;	
		if (angle < -60.0f && direction == 1) return;

		float new_pos_x = collider.getPosition().x + direction;		// every horizontal move is always 1px
		current_range_new = terrain.get_terrain()->at(collider.getPosition().x)->top(); // terrain.get_top_vertex_position_of_vertical_array_at_width(collider.getPosition().x);	// get the next vertical pixel range from terrain since we moved horizontally by 1px
		float new_pos_y = current_range_new;				// set our new vertical position to the range's topmost pixel

		collider.setPosition(new_pos_x, new_pos_y);
		collider.setRotation(angle);
	}

	tube.setPosition(collider.getPosition().x, collider.getPosition().y - 5);	//TODO remove magicnum
	//tube.setRotation(-45);		//TODO

	body.setPosition(collider.getPosition());
	body.setRotation(collider.getRotation());
}