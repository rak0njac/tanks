#pragma once
#include <SFML/Graphics.hpp>
#include "consts.h"
#include "NewVerticalLine.h"
#include <iostream>

class TerrainChunk {
public:
    TerrainChunk();
    ~TerrainChunk();
    void push(const int& position, NewVerticalLine* vec);
    void pop(const int& position);
    NewVerticalLine* at(const int &position);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    bool contains_vertex_at(const sf::Vector2f& position);
    void tidy();
    int count();
    bool is_falling;
    void move(const int &num_pixels, TerrainChunk &array, std::string direction = "down");
    void push(TerrainChunk &array);

private:
    std::vector<NewVerticalLine*> vec;

};
