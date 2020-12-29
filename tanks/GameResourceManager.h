#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <list>
class Player;
class Weapon;
class Projectile;
class GameResourceManager
{
public:
	std::vector<Player*> players;
	std::vector<Weapon*> weapon_pool;
	std::list<Projectile*> projectiles;
	std::vector<sf::Keyboard::Key> key_events;
	std::vector<sf::Mouse::Button> mouse_button_events;
	std::vector<sf::Mouse::Wheel> mouse_wheel_events;
	sf::Vector2f mousepos;
	GameResourceManager();
	~GameResourceManager();
};

