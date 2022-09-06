#include "TerrainEngine.h"

void TerrainEngine::logic() {
	main_terrain->logic();
	for (auto& t : other_terrains) {
		if (t != nullptr) {
			t->logic();
		}
	}
}

void TerrainEngine::render(sf::RenderTarget& target, sf::RenderStates& states) {
	main_terrain->draw(target, states);
	for (auto& t : other_terrains) {
		if (t != nullptr) {
			t->draw(target, states);
		}
	}
}