
#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <unordered_map>

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
	int getRangeNEW(int width);
	void ground_fall_by_range(sf::VertexArray va);
	Range* operator[](int);
	Range* operator[](float);

	sf::VertexArray vertices;
	std::vector<Range> ranges;
	sf::VertexArray NEW_vertices;
	std::unordered_map<int, sf::VertexArray> NEW_vertices_vertical;

private:
	void randomize(float roughness);
	void applyRange();
	void range_destroy_single(int x, int y);
	void ray_destroy_multi(sf::Vector2f origin, float angle, float other_angle, float thickness);
	sf::Texture terrainTexture;
	void colorRange(sf::Color c);

	bool hasFalling = false;
	

};




// NEW CODE

//#pragma once
//#include <vector>
//#include <SFML/Graphics.hpp>
//
//// structure that represents
//struct Range {
//	int min;
//	int max;
//	Range* next;
//};
//
//float squarePointDistance(sf::Vector2f pos1, sf::Vector2f pos2);
//
//class Terrain : public sf::Drawable
//{
//public:
//	Terrain();
//	~Terrain();
//	//void rayDestroy(sf::Vector2f origin, float angle, float thickness);
//	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
//	void destroy_circle(sf::Vector2i pos, int radius);
//	void groundFall();
//
//	sf::VertexArray vertices;
//
//private:
//	void displacement(float displace, float roughness);
//	void applyRange();
//	void range_destroy_single(int x, int y);
//	void ray_destroy_multi(sf::Vector2f origin, float angle, float other_angle, float thickness);
//	sf::Texture terrainTexture;
//
//	bool hasFalling = false;
//
//
//};
//
