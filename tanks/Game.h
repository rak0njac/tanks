#pragma once
#include <SFML/Graphics.hpp>
#include "Terrain.h"
#include "Player.h"
#include "HUD.h"

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



};

