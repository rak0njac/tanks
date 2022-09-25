#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "consts.h"

class VerticalVertexArray {
public:
	VerticalVertexArray(const int& horizontal_position, const int& vertical_position_bottom, const int& vertical_position_top);
	VerticalVertexArray(const std::vector<sf::Vertex>& vertices_to_move, const int& bottom, const int& top);
	~VerticalVertexArray();
	void push(const int& count); //number of pixels to add to the top
	void pop(const int& count); //number of pixels to delete from the top
	int peek(); //get top pixel position
	void move(const int& num_of_pixels); //Move all vertices by *num_of_pixels* pixels. > 0 for move up, < 0 for move down.
private:
	int horizontal_position;
	std::vector<sf::Vertex> vec;
};