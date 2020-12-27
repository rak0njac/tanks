#pragma once
#include <SFML/Graphics.hpp>
#include "Terrain.h"
#include "Player.h"
#include "HUD.h"
#include "Projectile.h"
#include <list>

class Game
{
public:
	Game();
	~Game();

	void run();
	void pool();
	void render();
	void update();
	void handlePress(sf::Keyboard::Key key, bool pressed);
	

private:
	sf::RenderWindow *window;
	sf::View gameView;
	sf::View hudView;
	Terrain* terrain;
	HUD* hud;
	sf::Vector2i mousepos;
	std::list<Projectile*> projectiles;



};

