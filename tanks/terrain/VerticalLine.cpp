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

void VerticalLine::push(const int& count)
{
    auto& _top = drawer.back();
    _top.position.y -= count;
    _top.texCoords.y -= count; // preserves texture coordinates
}

void VerticalLine::pop(int p_count)
{
    if(p_count < 1) return;

    int old_top = drawer.back().position.y;
    auto new_top = drawer.back().position.y + p_count;
    int cnt = 0;

    for(int i = 0; i < drawer.size(); ++i){
        if(drawer.at(i).position.y < new_top){
            cnt++;
        }
    }

    if(cnt >= drawer.size()){
        clear();
        return;
    }

    for(int i = 0; i < cnt - 1; i++){
        drawer.pop_back();
    }

    drawer.back().position.y = new_top;
    drawer.back().texCoords.y += p_count; // preserves texture coordinates
    drawer.at(1).texCoords.y = drawer.at(1).position.y; //HACK
}

void VerticalLine::pop_from_bottom(int p_count)
{
    if(p_count < 1) return;
    //std::cout << p_count << std::endl;

    auto new_bottom = drawer.front().position.y - p_count;
    int cnt = 0;

    for(int i = 0; i < drawer.size(); ++i){
        if(drawer.at(i).position.y > new_bottom){
            cnt++;
        }
    }

    if(cnt >= drawer.size()){
        clear();
        return;
    }

    for(int i = 0; i < cnt - 1; ++i){
        drawer.erase(drawer.begin());
    }

    drawer.front().position.y = new_bottom;
    drawer.front().texCoords.y -= p_count; // preserves texture coordinates



//    auto& _top = drawer.back();
//    auto& _next = drawer.at(1);
//    auto& _bottom = drawer.front();
//
//    while(_bottom.position.y - p_count <= _next.position.y && &_next != &_top){
//        p_count -= _bottom.position.y - _next.position.y;
//        drawer.erase(drawer.begin());
//
//        _bottom = drawer.back();
//        _next = drawer.at(1);
//    }
//
//    _bottom.position.y -= p_count;
//    _bottom.texCoords.y -= p_count * 2; // preserves texture coordinates
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
    auto _top = &drawer.back();
    auto _bottom = &drawer.front();

    //return _bottom->position.y - _top->position.y;

    return drawer.size();}

bool VerticalLine::contains_vertex_at(float position) {
    auto& _top = drawer.back();
    auto& _bottom = drawer.front();

    if(position > _top.position.y && position < _bottom.position.y)
        return true;
    return false;
}

bool VerticalLine::is_between(float p_bottom, float p_top) {
    auto& _top = drawer.back();
    auto& _bottom = drawer.front();

    if(_bottom.position.y < p_bottom && _top.position.y > p_top){
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
    int begin = 0;
    for(; begin < pLine->drawer.size(); begin++){
        if(new_bottom < pLine->drawer.at(begin).position.y) continue;
        begin--;
        break;
    }
    for(; begin < pLine->drawer.size(); begin++){
        drawer.emplace_back(pLine->drawer.at(begin));
    }
    pop_from_bottom(drawer.front().position.y - new_bottom);

    //std::cout << std::endl;
    for(auto& v : drawer){
        //std::cout << "VerticalLine::VerticalLine(float new_bottom, VerticalLine *pLine) " << v.position.y << std::endl;
    }
}
