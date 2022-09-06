#pragma once
#include <SFML/Graphics.hpp>
#include "Terrain.h"
#include "Player.h"
#include "HUD.h"
#include "Projectile.h"
#include <list>
#include "GameResourceManager.h"
#include "Parser.h"
#include "TerrainEngine.h"
class Game
{
public:
	Game();
	~Game();

	void run();
	void poll();
	void render();
	void logic();
	void init_weapons();
	void handlePress(sf::Keyboard::Key key, bool pressed);
	void handleMousePress(sf::Mouse::Button button, bool pressed);

	void handleMouseWheel(int scrolled);
	

private:
	GameResourceManager res;
	sf::RenderWindow *window;
	sf::View view;
	sf::View hudView;
	//Terrain* terrain;
	HUD* hud;
	sf::Vector2i mousepos;
	std::vector<Weapon> all_weapons;
	TerrainEngine te;



};

