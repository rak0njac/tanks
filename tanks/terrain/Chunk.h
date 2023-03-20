#pragma once
#include <SFML/Graphics.hpp>
#include "../consts.h"
#include "VerticalLine.h"
#include <iostream>

class Chunk {
public:
    Chunk();
    ~Chunk();
    void push(const int& position, VerticalLine* vec);
    void pop(const int& position);
    VerticalLine* at(const int &position);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    bool contains_vertex_at(const sf::Vector2f& position);
    void tidy();
    int count();
    bool is_falling;
    void move(const int &num_pixels, Chunk &array, std::string direction = "down");
    void push(Chunk &array);
    std::vector<VerticalLine*> vec;
    void set_color(sf::Color color);
private:
    

};
