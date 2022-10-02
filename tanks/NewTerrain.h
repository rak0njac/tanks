#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <HorizontalVertexArray.h>
#include <BoundingVertexArray.h>
#include "Random.h"
#include <iostream>

class NewTerrain: public sf::Drawable {
public:
    HorizontalVertexArray main_terrain;
    std::vector<HorizontalVertexArray> falling_terrains;

    NewTerrain();
    NewTerrain(const std::string &file);
    ~NewTerrain();

    void destroy_circle(sf::Vector2i pos_projectile, int radius);
    void logic();
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    HorizontalVertexArray* get_terrain();
    std::vector<HorizontalVertexArray> get_terrains();
private:
    sf::Texture texture;
    sf::VertexArray debug_boundaries;

    void randomize(int roughness);
    void fall();
    std::vector<std::vector<sf::Vertex>> generate_half_circle(int m_radius, int m_pos_x, int m_pos_y);

    void tidy();

    void destroy_circle(sf::Vector2f &pos_lower, sf::Vector2f &pos_upper, HorizontalVertexArray &j,
                        HorizontalVertexArray &hva_upper, HorizontalVertexArray &hva_lower);
};