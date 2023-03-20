#include "VerticalLine.h"
#include <iostream>
#include <array>

VerticalLine::VerticalLine(const int& horizontal_position, const int& vertical_position_bottom, const int& vertical_position_top)
        : horizontal_position(horizontal_position)
{
    if(vertical_position_top >= vertical_position_bottom){
        std::cout << "WARNING :: VerticalLine :: vertical_position_top is bigger than or equal to vertical_position_bottom" << std::endl;
        return;
    }
    sf::Vertex _top = sf::Vertex(sf::Vector2f(horizontal_position, vertical_position_top));
    sf::Vertex _bottom = sf::Vertex(sf::Vector2f(horizontal_position, vertical_position_bottom));

    _top.texCoords = sf::Vector2f(horizontal_position, vertical_position_top);
    _bottom.texCoords = sf::Vector2f(horizontal_position, vertical_position_bottom);

    drawer.emplace_back(_bottom);
    drawer.emplace_back(_top);
}

//void VerticalLine::push(const int& count)
//{
//    auto& _top = drawer.back();
//    _top.position.y -= count;
//    _top.texCoords.y -= count; // preserves texture coordinates
//}

void VerticalLine::pop(int p_count)
{
    if(p_count < 1) return;

    auto new_top = drawer.back().position.y + p_count;


    while (drawer.size() > 2 && drawer.at(drawer.size() - 2).position.y < new_top) {
        int to_remove = (drawer.back().position.y - drawer.at(drawer.size() - 2).position.y) * -1;
        p_count -= to_remove;
        drawer.pop_back();
    }

    drawer.back().position.y = new_top;
    drawer.back().texCoords.y += p_count; // preserves texture coordinates
}

void VerticalLine::pop_from_bottom(int new_bottom)
{
    auto old_bottom = drawer.at(0).position.y;
    auto p_count = old_bottom - new_bottom;

    if (p_count < 1) return;

    while (drawer.size() > 2 && drawer.at(2).position.y > new_bottom) {
        int to_remove = (drawer.front().position.y - drawer.at(1).position.y);
        p_count -= to_remove;
        drawer.erase(drawer.begin(), drawer.begin() + 2);
    }

    drawer.front().position.y = new_bottom;
    drawer.front().texCoords.y -= p_count; // preserves texture coordinates
}

int VerticalLine::top()
{
    auto& _top = drawer.back();
    return _top.position.y;
}

int VerticalLine::bottom()
{
    auto& _bottom = drawer.front();
    return _bottom.position.y;
}

void VerticalLine::move(const int& num_of_pixels, std::string direction)
{
    for(auto& v : drawer){
        v.position.y++;
    }
}

VerticalLine::~VerticalLine() {

}

void VerticalLine::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(drawer.data(), drawer.size(), sf::PrimitiveType::Lines, states);
}

std::vector<sf::Vertex>* VerticalLine::get_vector() {
    return &drawer;
}

void VerticalLine::set_color(const sf::Color &color) {
    for(auto& v : drawer){
        v.color = color;
    }
}

void VerticalLine::clear() {
    drawer.clear();
}

int VerticalLine::count() {
    return drawer.size();
}

bool VerticalLine::contains_vertex_at(float position) {
    if (count() == 0) return false;
    auto& _top = drawer.back();
    auto& _bottom = drawer.front();

    if(position > _top.position.y && position < _bottom.position.y)
        return true;
    return false;
}

bool VerticalLine::is_between(float p_bottom, float p_top) {
    if (drawer.size() == 0) return false;
    auto& _top = drawer.back();
    auto& _bottom = drawer.front();

    if(_bottom.position.y <= p_bottom && _top.position.y >= p_top){
        return true;
    }
    return false;
}

sf::Vector2f VerticalLine::get_tex_coords(int index) {
    return drawer.at(index).texCoords;
}

void VerticalLine::push(std::vector<sf::Vertex> vec) {
    for(auto& v : vec){
        //v.position.y--;
        v.color = sf::Color::White;
        drawer.emplace_back(v);
    }
}

VerticalLine::VerticalLine(float new_bottom, VerticalLine *pLine) {
    auto vec = pLine->get_vector();
    drawer = *vec;

    //while (drawer.size() > 2 && drawer.at(1).position.y > new_bottom) {
    //    int to_remove = (drawer.front().position.y - drawer.at(1).position.y) * -1;
    //    drawer.erase(drawer.begin());
    //}



    //int begin = 0;
    //for(; begin < pLine->drawer.size(); begin++){
    //    if(new_bottom < pLine->drawer.at(begin).position.y) continue;
    //    begin--;
    //    break;
    //}
    //for(; begin < pLine->drawer.size(); begin++){
    //    drawer.emplace_back(pLine->drawer.at(begin));
    //}
    pop_from_bottom(new_bottom);

    //std::cout << std::endl;
    for(auto& v : drawer){
        //std::cout << "VerticalLine::VerticalLine(float new_bottom, VerticalLine *pLine) " << v.position.y << std::endl;
    }
}
