#include "Weapon.h"
#include <math.h>
const float pi = std::acos(-1);

Weapon::Weapon()
{
	type = WeaponType::STANDARD;
}

Weapon::Weapon(WeaponType t)
{
	type = t;
}

Weapon::Weapon(WeaponType t, float expls, float cd, float sp)
{
	type = t;
	expl_size = expls;
	cooldown = cd;
	speed = sp;
}

Weapon::~Weapon()
{
}

void Weapon::fire(sf::Vector2f pos, float angle,  std::list<Projectile*>& projectiles) {
	switch (type) {
	case WeaponType::STANDARD: fire_standard(pos, angle, projectiles); break;

	}
}
void Weapon::fire_standard(sf::Vector2f pos, float angle, std::list<Projectile*>& projectiles) {
	sf::Vector2f ppos;
	ppos.x = pos.x + spawn_offset * (cos(angle * pi / 180));
	ppos.y = pos.y + spawn_offset * (sin(angle * pi / 180));
	Projectile* proj = new Projectile(ppos, angle, speed, expl_size);
	projectiles.push_back(proj);
}

void Weapon::clean() {

}