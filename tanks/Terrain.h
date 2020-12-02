#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

struct Range {
	int min;
	int max;
	Range* next;
};

float squarePointDistance(sf::Vector2f pos1, sf::Vector2f pos2);

class Terrain: public sf::Drawable
{
public:
	Terrain();
	~Terrain();
	Terrain(const std::string& file);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void destroy(sf::Vector2i pos, int radius);
	void groundFall();
	void groundFallThreaded(int thread);

	
	sf::VertexArray vArray;
	std::vector<Range> ranges;

private:
	void displacement(float displace, float roughness);
	void applyRange();

	sf::Texture terrainTexture;

	bool hasFalling = false;
	

};

