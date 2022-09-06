#pragma once
#include "Terrain.h"

class TerrainEngine {
public:
	 Terrain* main_terrain;
	 std::vector<Terrain*> other_terrains;

	 void logic();

	 void render(sf::RenderTarget& target, sf::RenderStates& states);
};