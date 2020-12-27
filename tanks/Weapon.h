#pragma once
#include <SFML/Graphics.hpp>
#include "Projectile.h"
#include <list>
class Game;
class Projectile;
enum class WeaponType {
	STANDARD
};
class Weapon
{
public:
	Weapon();
	Weapon(WeaponType t);
	Weapon(WeaponType t, float expl_size, float cd, float speed);
	~Weapon();
	void fire(sf::Vector2f pos, float angle,  std::list<Projectile*>& projectiles);
	void clean();

	float speed = 3;
	float expl_size = 8;
	float cooldown = 1;
	float spawn_offset = 5;
private:
	WeaponType type;
	

	void fire_standard(sf::Vector2f pos, float angle,  std::list<Projectile*>& projectiles);

};

