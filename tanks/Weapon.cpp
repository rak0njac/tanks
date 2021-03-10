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

Weapon::Weapon(std::string n, WeaponType t, float expls, float cd, float sp, float pr_rad, float dr_rate)
{
	name = n;
	type = t;
	expl_size = expls;
	cooldown = cd;
	speed = sp;
	proj_rad = pr_rad;
	drop_rate = dr_rate;
}

Weapon::~Weapon()
{
}

void Weapon::fire(sf::Vector2f pos, float angle,  std::list<Projectile*>& projectiles) {
	if (last_fired.getElapsedTime().asSeconds() < cooldown) return;
	last_fired.restart();
	switch (type) {
	case WeaponType::STANDARD: fire_standard(pos, angle, projectiles); break;

	}
}
void Weapon::fire_standard(sf::Vector2f pos, float angle, std::list<Projectile*>& projectiles) {
	sf::Vector2f ppos;
	ppos.x = pos.x + spawn_offset * (cos(angle * pi / 180));
	ppos.y = pos.y + spawn_offset * (sin(angle * pi / 180));
	Projectile* proj = new Projectile(ppos, angle, speed, expl_size, proj_rad);
	proj->drop_rate = drop_rate;
	proj->drop_max = drop_max;
	projectiles.push_back(proj);
}

void Weapon::clean() {

}