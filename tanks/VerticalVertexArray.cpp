#pragma once
#include "VerticalVertexArray.h"

VerticalVertexArray::VerticalVertexArray(const int& horizontal_position, const int& vertical_position_bottom, const int& vertical_position_top)
	: horizontal_position(horizontal_position)
{
	push(vertical_position_bottom - vertical_position_top);
}

VerticalVertexArray::VerticalVertexArray(const std::vector<sf::Vertex>& vertices_to_move, const int& bottom, const int& top) {
	vec = std::vector<sf::Vertex>(std::make_move_iterator(vertices_to_move.begin() + bottom), std::make_move_iterator(vertices_to_move.begin() + top));
}

void VerticalVertexArray::push(const int& count)
{
	int top = peek();
	for (int i = 0; i < count, top > 0; ++i, --top) {
		vec.emplace_back(sf::Vertex(sf::Vector2f(horizontal_position, top)));
	}
}

void VerticalVertexArray::pop(const int& count)
{
	for (int i = 0; i < count, i < vec.size(); ++i) {
		vec.pop_back();
	}
}

int VerticalVertexArray::peek()
{
	if (vec.empty()) return const_screen_height;
	return vec.back().position.y;
}

void VerticalVertexArray::move(const int& num_of_pixels)
{
	for (int i = 0; i < vec.size(); ++i) {
		vec.at(i).position.y += num_of_pixels;
	}
}

VerticalVertexArray::~VerticalVertexArray() {

}
