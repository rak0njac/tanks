#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>

class BoundingVertexArray : public sf::Drawable{
public:
	BoundingVertexArray();
	~BoundingVertexArray();
	void insert(const sf::Vector2f& position);
	bool exists(const sf::Vector2f& position);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    std::vector<sf::Vertex> vec;
};
