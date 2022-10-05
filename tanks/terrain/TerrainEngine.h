#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include "BoundingVertexArray.h"
#include "Random.h"
#include "TerrainChunk.h"
#include <iostream>

class TerrainEngine: public sf::Drawable {
public:
    TerrainChunk main_terrain;
    std::vector<TerrainChunk> falling_terrains;

    TerrainEngine();
    TerrainEngine(const std::string &file);
    ~TerrainEngine();

    void destroy_circle(sf::Vector2i pos_projectile, int radius);
    void logic();
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    TerrainChunk* get_main_terrain();
    std::vector<TerrainChunk> get_all_terrains();
private:
    sf::Texture texture;
    sf::VertexArray debug_boundaries;
    sf::VertexArray va;

    void randomize(int roughness);
    void fall();
    std::vector<std::vector<sf::Vertex>> generate_half_circle(int m_radius, int m_pos_x, int m_pos_y);

    void tidy();

    void destroy_circle(sf::Vector2f &pos_lower, sf::Vector2f &pos_upper, TerrainChunk &terrain_chunk,
                        TerrainChunk &new_terrain_chunk_upper);
};