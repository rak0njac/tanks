#include "legacy_VerticalVertexArray.h"
#include <iostream>

legacy_VerticalVertexArray::legacy_VerticalVertexArray(const int& horizontal_position, const int& vertical_position_bottom, const int& vertical_position_top)
	: horizontal_position(horizontal_position), _bottom(vertical_position_bottom), _top(vertical_position_top)
{
	push(vertical_position_bottom - vertical_position_top + 1);
}

legacy_VerticalVertexArray::legacy_VerticalVertexArray(const std::vector<sf::Vertex>& vertices_to_move, const int& bottom, const int& top) {
	vec = std::vector<sf::Vertex>(std::make_move_iterator(vertices_to_move.begin() + bottom), std::make_move_iterator(vertices_to_move.begin() + top));
}

void legacy_VerticalVertexArray::push(const int& count)
{
	int l_top = top();
	for (int i = 0; i < count && l_top > 0; ++i, --l_top) {
        //std::cout << l_top << std::endl;
        sf::Vertex ver(sf::Vector2f(horizontal_position, l_top));
        ver.texCoords = sf::Vector2f(horizontal_position * 2, l_top * 2);
		vec.emplace_back(ver);
	}
    //std::cout << std::endl;
}

void legacy_VerticalVertexArray::pop(int count)
{
    if (count > vec.size() && count > 0) count = vec.size();    //clean this shit

    for (int i = 0; i < count; ++i) {
		vec.pop_back();
	}
}

int legacy_VerticalVertexArray::top()
{
	if (vec.empty()) return _bottom;
	return vec.back().position.y;
}

int legacy_VerticalVertexArray::bottom()
{
    if (vec.empty()) return _bottom;
    return vec.front().position.y;
}

void legacy_VerticalVertexArray::move(const int& num_of_pixels, std::string direction)
{
	for (auto & i : vec) {
		i.position.y += num_of_pixels;
	}
}

legacy_VerticalVertexArray::~legacy_VerticalVertexArray() {

}

void legacy_VerticalVertexArray::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    //target.draw(vec.data(), vec.size(), sf::PrimitiveType::Points, states);
}

std::vector<sf::Vertex> *legacy_VerticalVertexArray::get_vector() {
    return &vec;
}

void legacy_VerticalVertexArray::set_color(const sf::Color &color) {
    for(auto& v : vec){
        v.color = color;
    }
}

void legacy_VerticalVertexArray::clear() {
    vec.clear();
}

int legacy_VerticalVertexArray::count() {
    return vec.size();
}

bool legacy_VerticalVertexArray::contains_vertex_at(float position) {
    if(position >= top() && position <= bottom())
        return true;
    return false;
}

bool legacy_VerticalVertexArray::is_between(float p_bottom, float p_top) {
    if(bottom() < p_bottom && top() > p_top){
        return true;
    }
    return false;
}
