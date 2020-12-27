#include "Game.h"
const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
sf::Clock clickClock;
static std::vector<Player> players;

Game::Game()
{
	window = new sf::RenderWindow(sf::VideoMode(800, 600), "tanks");
	terrain = new Terrain("assets\\ground.jpg");
	hud = new HUD("assets\\Lato-Regular.ttf");
	gameView.setCenter(400, 300);
	gameView.setSize(sf::Vector2f(800, 600));
	gameView.setViewport(sf::FloatRect(0.f, 0.f, 1, 0.75f));

	players.push_back(Player());
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

		}
	}
}

void Game::render() {
	window->clear();
	window->setView(gameView);
	window->draw(*terrain);
	for (int i = 0; i < players.size(); i++) {
		window->draw(players[i]);
	}
	
	for (Projectile* i: projectiles) {
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
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window->hasFocus() && clickClock.getElapsedTime().asSeconds() > 0.1f)
	{
		players[0].shoot(projectiles);
		clickClock.restart();
	}
	terrain->groundFall();
	for (int i = 0; i < players.size(); i++) {
		players[i].move(*terrain, sf::Vector2f(0,0), window->mapPixelToCoords(mousepos, gameView));
	}
	std::list<Projectile*>::iterator it = projectiles.begin();
	while (it != projectiles.end()) {
		if ((*it)->destroyed) {
			delete (*it);
			it = projectiles.erase(it);
		}
		else {
			(*it)->move(*terrain);
			it++;
		}
	}
	for (Projectile* i: projectiles) {
		i->move(*terrain);
	}
}

void Game::handlePress(sf::Keyboard::Key key, bool pressed)
{
	switch (key) {
	case sf::Keyboard::D: players[0].movingRight = pressed; break;
	case sf::Keyboard::A: players[0].movingLeft = pressed; break;
	}
}
