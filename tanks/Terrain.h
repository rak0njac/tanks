#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

// structure that represents
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
	void rayDestroy(sf::Vector2f origin, float angle,float thickness);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void destroy_circle(sf::Vector2i pos, int radius);
	void groundFall();
	void groundFallThreaded(int thread);
	Range* operator[](int);
	Range* operator[](float);

	sf::VertexArray vertices;
	std::vector<Range> ranges;

private:
	void displacement(float displace, float roughness);
	void applyRange();
	void range_destroy_single(int x, int y);
	void ray_destroy_multi(sf::Vector2f origin, float angle, float other_angle, float thickness);
	sf::Texture terrainTexture;

	bool hasFalling = false;
	

};

