//#pragma once
//#include <SFML/Graphics.hpp>
//#include "Terrain.h"
//#include "Weapon.h"
//#include <list>
//#include "GameResourceManager.h"
//class Game;
//const float const_falling_speed = 2.f;
//const float const_move_speed = 1.f;
//class Player: public sf::Drawable
//{
//public:
//	Player();
//	~Player();
//	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
//	void shoot(Weapon &weapon);
//	void shoot( std::list<Projectile*>& projectiles);
//	void add_wep(Weapon wep);
//	void change_wep(int);
//	void change_wep_abs(int);
//	void move(const Terrain& terrain, sf::Vector2f mv,const sf::Vector2f &mousepos);
//
//	void update(const Terrain& terrain, sf::Vector2f mv, const sf::Vector2f &mousepos,
//		GameResourceManager& grm);
//
//	bool movingLeft = false;
//	bool movingRight = false;
//	bool firing = false;
//private:
//	sf::RectangleShape body;
//	sf::RectangleShape tube;
//	sf::CircleShape collisionCircle;
//	sf::Vector2f pos;
//	sf::Vector2f movementVec;
//	std::vector<Weapon> weapons;
//	int currentWeapon = 0;
//
//	float rotation = 0;
//	
//	bool falling = true;
//
//};
//
