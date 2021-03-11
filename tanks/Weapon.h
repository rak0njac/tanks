#pragma once
#include <SFML/Graphics.hpp>
#include "Projectile.h"
#include <list>
class Game;
class Projectile;
enum class WeaponType {
	STANDARD,
	LAZOR
};
class Weapon
{
public:
	Weapon();
	Weapon(WeaponType t);
	Weapon(WeaponType t, float expl_size, float cd, float speed);
	Weapon(std::string name, WeaponType t, float expl_size, float cd, float speed, float proj_rad, float drop_rate);
	//Weapon(WeaponType t, float expl_size, float cd, float speed,float proj_radius, float spawn_offset);
	~Weapon();
	void fire(sf::Vector2f pos, float angle,  std::list<Projectile*>& projectiles);
	void clean();

	std::string name;
	float speed = 3;
	float expl_size = 8;
	float cooldown = 1;
	float spawn_offset = 5;
	float proj_rad = 3;
	float drop_rate = 0;
	float drop_max = 0;
private:
	WeaponType type;
	sf::Clock last_fired;

	void fire_standard(sf::Vector2f pos, float angle,  std::list<Projectile*>& projectiles);
	void fire_lazor(sf::Vector2f pos, float angle, std::list<Projectile*>& projectiles);
};

