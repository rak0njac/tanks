#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class BoundingVertexArray {
public:
	BoundingVertexArray();
	~BoundingVertexArray();
	void insert(sf::Vector2f& position);
	bool exists_vertex_at(sf::Vector2f& position);
private:
	std::vector<sf::Vertex> vec;
};