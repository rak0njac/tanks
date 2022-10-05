#pragma once
#include <SFML/Graphics.hpp>
#include "consts.h"
#include "legacy_VerticalVertexArray.h"
#include <iostream>

class legacy_HorizontalVertexArray {
public:
	legacy_HorizontalVertexArray();
	~legacy_HorizontalVertexArray();
	void push(const int& position, legacy_VerticalVertexArray* vec);
	void pop(const int& position);
    legacy_VerticalVertexArray* at(const int &position);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    bool contains_vertex_at(const sf::Vector2f& position);
    void tidy();
    int count();
    bool is_falling;
    void move(const int &num_pixels, legacy_HorizontalVertexArray &array, std::string direction = "down");
    void push(legacy_HorizontalVertexArray &array);

private:
	std::vector<legacy_VerticalVertexArray*> vec;

};
