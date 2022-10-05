#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <array>
#include "consts.h"

class NewVerticalLine: public sf::Drawable{
public:
    NewVerticalLine(const int& horizontal_position, const int& vertical_position_bottom, const int& vertical_position_top);
    NewVerticalLine(const std::vector<sf::Vertex>& vertices_to_move, const int& bottom, const int& top);
    ~NewVerticalLine();
    void push(const int& count); //number of pixels to add to the _top
    void pop(int count); //number of pixels to delete from the _top
    int top(); //get _top pixel position
    int bottom(); //get _bottom pixel position
    void move(const int& num_of_pixels, std::string direction = "up"); //Move all vertices by *num_of_pixels* pixels. > 0 for move up, < 0 for move down.
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    std::array<sf::Vertex, 2> * get_vector();
    void set_color(const sf::Color& color);
    void clear();
    int count();

    bool contains_vertex_at(float d);

    bool is_between(float d, float d1);

private:
    sf::Vertex _bottom;
    sf::Vertex _top;
    std::array<sf::Vertex, 2> drawer;
    int horizontal_position;
};
