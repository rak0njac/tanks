#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <unordered_set>

float squarePointDistance(sf::Vector2f pos1, sf::Vector2f pos2);

class Terrain: public sf::Drawable
{
public:
	Terrain();
	~Terrain();
	Terrain(const std::string& file);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void destroy_circle(sf::Vector2i pos, int radius);
	void logic();
	void LEGACY_groundFallThreaded(int thread);
	bool contains_vertex_at(int width, int height);
	int get_top_vertex_position_of_vertical_array_at_width(int width);
	bool contains_vertex_that_needs_to_fall(int width);
	void fall_vertex(std::unordered_set<int> ttf);

	//std::vector<sf::Vertex> vertical_vertices; //int is global Y position of sf::Vertex
	std::vector<std::vector<sf::Vertex>*> horizontal_map_of_vertical_vertices; //int is global X position of given vertical_vertices value
	Terrain(std::vector<std::vector<sf::Vertex>>);

private:
	void randomize(float roughness);
	void new_destroy_single(int x, int y);
	sf::Texture terrainTexture;
	std::vector<sf::Vertex*> arr;
	std::unordered_set<int> terrain_to_fall;
	void regenerate_terrain(std::unordered_set<int> ttf);
	std::vector<int> line_to_displace;

	bool hasFalling = false;
};