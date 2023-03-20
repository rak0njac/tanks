#include "BoundingVertexArray.h"

BoundingVertexArray::BoundingVertexArray() {

}

BoundingVertexArray::~BoundingVertexArray() {

}

void BoundingVertexArray::insert(const sf::Vector2f &position) {
    vec.emplace_back(position);
}

bool BoundingVertexArray::exists(const sf::Vector2f &position) {
    return false;
}

void BoundingVertexArray::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(vec.data(), vec.size(), sf::PrimitiveType::Points, states);
}
