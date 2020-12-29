#include "Game.h"
const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
sf::Clock clickClock;


Game::Game()
{
	window = new sf::RenderWindow(sf::VideoMode(800, 600), "tanks");
	terrain = new Terrain("assets\\ground.jpg");
	hud = new HUD("assets\\Lato-Regular.ttf");
	gameView.setCenter(400, 300);
	gameView.setSize(sf::Vector2f(800, 600));
	gameView.setViewport(sf::FloatRect(0.f, 0.f, 1, 0.75f));

	grm.players.push_back(new Player());
	//grm.players[0].add_wep(Weapon());
	init_weapons();
	//players.push_back(Player());
	/*hudView.setSize(sf::Vector2f(800, 150));
	hudView.setCenter(400, 1000);
	hudView.setViewport(sf::FloatRect(0.f, 0.25f, 1, 1));*/
}

void Game::init_weapons() {
	Weapon* w = new Weapon(WeaponType::STANDARD, 8, 0.1, 10);
	w->proj_rad = 3;
	w->drop_rate = 10;
	grm.weapon_pool.push_back(w);

	w =  new Weapon(WeaponType::STANDARD, 50, 3, 3);
	w->proj_rad = 7;
	grm.weapon_pool.push_back(new Weapon());
	grm.weapon_pool.push_back(w);

	w = new Weapon(WeaponType::STANDARD, 9, 0,60);
	w->proj_rad = 3;
	grm.weapon_pool.push_back(w);

	w = new Weapon(WeaponType::STANDARD, 9, 0, 5);
	w->proj_rad = 3;
	w->drop_rate = 2;
	grm.weapon_pool.push_back(w);
	for (int i = 0; i < grm.weapon_pool.size(); i++) {
		grm.players[0]->add_wep(*grm.weapon_pool[i]);
	}
}

Game::~Game()
{
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceUpdate = sf::Time::Zero;
	sf::RectangleShape rect(sf::Vector2f(200.f,200.f));
	rect.setFillColor(sf::Color::Red);
	
	while (window->isOpen()) {
		pool();
		timeSinceUpdate += clock.restart();
		while (timeSinceUpdate >= TimePerFrame) {
			pool();
			timeSinceUpdate -= TimePerFrame;
			update();
		}

		render();
	}
}

void Game::pool() {
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window->close();
		if (event.type == sf::Event::KeyPressed) {
			handlePress(event.key.code, true);
		}
		if (event.type == sf::Event::KeyReleased) {
			handlePress(event.key.code, false);
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			handleMousePress(event.mouseButton.button, true);
		}
		if (event.type == sf::Event::MouseButtonReleased)
		{
			handleMousePress(event.mouseButton.button, false);
		}
		if (event.type == sf::Event::MouseWheelScrolled)
		{
			handleMouseWheel(event.mouseWheelScroll.delta);
		}
	}
}

void Game::render() {
	window->clear();
	window->setView(gameView);
	window->draw(*terrain);
	for (int i = 0; i < grm.players.size(); i++) {
		window->draw(*grm.players[i]);
	}
	
	for (Projectile* i: grm.projectiles) {
		window->draw(*i);
	}
	//window->setView(hudView);
	window->setView(window->getDefaultView());
	window->draw(*hud);
	window->display();
}

void Game::update() {
	mousepos = sf::Mouse::getPosition(*window);
	/*if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window->hasFocus() && clickClock.getElapsedTime().asSeconds() > 0.01f)
	{
		clickClock.restart();
		sf::Vector2f worldPos = window->mapPixelToCoords(mousepos, gameView);
		terrain->destroy(sf::Vector2i(worldPos.x,worldPos.y), 20);
	}*/
	/*if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window->hasFocus() && clickClock.getElapsedTime().asSeconds() > 0.1f)
	{
		grm.players[0]->shoot(grm.projectiles);
		clickClock.restart();
	}*/
	terrain->groundFall();
	for (int i = 0; i < grm.players.size(); i++) {
		grm.players[i]->update(*terrain, sf::Vector2f(0,0), window->mapPixelToCoords(mousepos, gameView), grm);
	}
	std::list<Projectile*>::iterator it = grm.projectiles.begin();
	while (it != grm.projectiles.end()) {
		if ((*it)->destroyed) {
			delete (*it);
			it = grm.projectiles.erase(it);
		}
		else {
			(*it)->move(*terrain);
			it++;
		}
	}
	for (Projectile* i: grm.projectiles) {
		i->move(*terrain);
	}
}

void Game::handlePress(sf::Keyboard::Key key, bool pressed)
{
	switch (key) {
	case sf::Keyboard::D: grm.players[0]->movingRight = pressed; break;
	case sf::Keyboard::A: grm.players[0]->movingLeft = pressed; break;
	}
}

void Game::handleMousePress(sf::Mouse::Button button, bool pressed)
{
	switch (button) {
	case sf::Mouse::Left: grm.players[0]->firing = pressed; break;
	}
}

void Game::handleMouseWheel(int scrolled)
{
	grm.players[0]->change_wep(scrolled);
}
