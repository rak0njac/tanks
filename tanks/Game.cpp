#include "Game.h"
#include <iostream>
const sf::Time time_per_frame = sf::seconds(1.0f / 60.0f);
//sf::Clock tick;


Game::Game()
{
	window = new sf::RenderWindow(sf::VideoMode(800, 600), "tanks");
	//TerrainEngine te;
	te.main_terrain = new Terrain("assets\\ground.jpg");
	//terrain = new Terrain("assets\\ground.jpg");
	hud = new HUD("assets\\Lato-Regular.ttf");
	view.setCenter(window->getSize().x / 2, window->getSize().y / 2);
	//view.setSize(window->getSize().x, window->getSize().y * 0.75f);
	//view.setViewport(sf::FloatRect(0, 0, 1, 0.75f));
	view.setSize(window->getSize().x, window->getSize().y);
	view.setViewport(sf::FloatRect(0, 0, 1, 1));

	window->setView(view);
	//window.setVerticalSyncEnabled(true)
	//window->setFramerateLimit(60);

	res.players.push_back(new Player());
	//grm.players.push_back(new Player());
	all_weapons = Parser::parse("weapons.txt");
	for (int i = 0; i < all_weapons.size(); i++) {
		res.players[0]->add_weapon(all_weapons[i]);
		//grm.players[1]->add_wep(all_weapons[i]);
	}
	//grm.players[0].add_wep(Weapon());
	//init_weapons();
	//players.push_back(Player());
	/*hudView.setSize(sf::Vector2f(800, 150));
	hudView.setCenter(400, 1000);
	hudView.setViewport(sf::FloatRect(0.f, 0.25f, 1, 1));*/
}

Game::~Game()
{
}

void Game::run()
{
	sf::Clock tick;
	sf::Time time_since_update = sf::Time::Zero;
	//sf::RectangleShape rect(sf::Vector2f(200.f,200.f));
	//rect.setFillColor(sf::Color::Red);
	
	while (window->isOpen()) {





		poll();
		time_since_update += tick.restart();
		if (time_since_update >= time_per_frame) {
			poll();
			//logic();
			time_since_update = sf::Time::Zero;// -= time_per_frame;
		}
		//std::cout << time_since_update.asMilliseconds() << std::endl;
		render();
	}
}

void Game::poll() {
	sf::Event event;
	if (window->pollEvent(event))
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
	logic();

	sf::Vertex rectangle[] =
	{
		sf::Vertex(sf::Vector2f(0,0), sf::Color::Black),
		sf::Vertex(sf::Vector2f(0,600), sf::Color::Blue),
		sf::Vertex(sf::Vector2f(800,600), sf::Color::Blue),
		sf::Vertex(sf::Vector2f(800,0), sf::Color::Black),
	};




	window->clear();
	window->setView(view);
	window->draw(rectangle, 4, sf::Quads);
	// MOVE ALL DRAWINGS TO THEIR RESPECTIVE CLASSES
	window->draw(*te.main_terrain);
	for (auto& a : te.other_terrains) {
		window->draw(*a);
	}
	for (int i = 0; i < res.players.size(); i++) {
		window->draw(*res.players[i]);
	}
	
	for (Projectile* i: res.projectiles) {
		window->draw(*i);
	}
	//window->setView(hudView);
	window->setView(window->getDefaultView());
	//window->draw(*hud);
	//
	window->display();
}

void Game::logic() {
	te.logic();
	//for (auto& a : te.other_terrains) {
	//	window->draw(*a);
	//}
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
	//terrain->logic();
	for (int i = 0; i < res.players.size(); i++) {
		res.players[i]->logic(*te.main_terrain, sf::Vector2f(0,0), window->mapPixelToCoords(mousepos, view), res);
	}
	//for (Projectile* p : res.projectiles) {
	//	if (p->destroyed) {
	//		delete p;
	//		p = nullptr;
	//	}
	//	else {
	//		p->move(*terrain);
	//	}
	//}

	std::list<Projectile*>::iterator it = res.projectiles.begin();
	while (it != res.projectiles.end()) {
		if ((*it)->destroyed) {
			delete (*it);
			it = res.projectiles.erase(it);
		}
		else {
			(*it)->move(*te.main_terrain);
			it++;
		}
	}
	for (Projectile* i: res.projectiles) {
		i->move(*te.main_terrain);
	}

	//render();
}

void Game::handlePress(sf::Keyboard::Key key, bool pressed)
{
	switch (key) {
	case sf::Keyboard::D: res.players[0]->direction = pressed; break;
	case sf::Keyboard::A: res.players[0]->direction = pressed * -1; break;
	}
}

void Game::handleMousePress(sf::Mouse::Button button, bool pressed)
{
	switch (button) {
	case sf::Mouse::Left: res.players[0]->firing = pressed; break;
	}
}

void Game::handleMouseWheel(int scrolled)
{
	res.players[0]->change_weapon(scrolled);
}