#pragma once
#include <SFML/Graphics.hpp>
#include "consts.h"
#include "VerticalVertexArray.h"
#include <iostream>

class HorizontalVertexArray {
public:
	HorizontalVertexArray();
	~HorizontalVertexArray();
	void push(const int& position, VerticalVertexArray* vec);
	void pop(const int& position);
    VerticalVertexArray* at(const int &position);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void prepare_for_drawing();
    bool contains_vertex_at(const sf::Vector2f& position);
    bool contains_vertex_between(const sf::Vector2f& bottom, const sf::Vector2f& top);
    void tidy();
    int count();
    bool is_main;
    bool is_falling;
    void move(const int &num_pixels, HorizontalVertexArray &array, std::string direction = "down");

    void push(HorizontalVertexArray &array);

private:
	std::vector<VerticalVertexArray*> vec;
    sf::VertexArray drawer;

};
