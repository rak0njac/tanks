#include "NewVerticalLine.h"
#include <iostream>
#include <array>

NewVerticalLine::NewVerticalLine(const int& horizontal_position, const int& vertical_position_bottom, const int& vertical_position_top)
        : horizontal_position(horizontal_position)
{
    _top = sf::Vertex(sf::Vector2f(horizontal_position, vertical_position_top));
    _bottom = sf::Vertex(sf::Vector2f(horizontal_position, vertical_position_bottom));

    _top.texCoords = sf::Vector2f(horizontal_position * 2, vertical_position_top * 2);
    _bottom.texCoords = sf::Vector2f(horizontal_position * 2, vertical_position_bottom * 2);

    drawer = {&_top, &_bottom};
}

void NewVerticalLine::push(const int& count)
{
    _top.position.y -= count;
    _top.texCoords.y -= count * 2; // preserves texture coordinates
}

void NewVerticalLine::pop(int count)
{
    if(_top.position.y + count <= const_screen_height){
        _top.position.y += count;
        _top.texCoords.y += count * 2; // preserves texture coordinates
    }
}

int NewVerticalLine::top()
{
    return _top.position.y;
}

int NewVerticalLine::bottom()
{
    return _bottom.position.y;
}

void NewVerticalLine::move(const int& num_of_pixels, std::string direction)
{
    _top.position.y++;
    _bottom.position.y++;
}

NewVerticalLine::~NewVerticalLine() {

}

void NewVerticalLine::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(*drawer.data(), 2, sf::PrimitiveType::Lines, states);
}

std::array<sf::Vertex*, 2> *NewVerticalLine::get_vector() {
    return &drawer;
}

void NewVerticalLine::set_color(const sf::Color &color) {
    _top.color = color;
    _bottom.color = color;
}

void NewVerticalLine::clear() {
    _top.position.y = 0;
    _bottom.position.y = 0;
}

int NewVerticalLine::count() {
    return _bottom.position.y - _top.position.y;
}

bool NewVerticalLine::contains_vertex_at(float position) {
    if(position >= _top.position.y && position <= _bottom.position.y)
        return true;
    return false;
}

bool NewVerticalLine::is_between(float p_bottom, float p_top) {
    if(_bottom.position.y < p_bottom && _top.position.y > p_top){
        return true;
    }
    return false;
}

sf::Vector2f NewVerticalLine::get_tex_coords(int index) {
    return drawer.at(index)->texCoords;
}

void NewVerticalLine::push(sf::Vertex &p_top) {

}
