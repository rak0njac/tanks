#include "Terrain.h"
#include <math.h>
#include <algorithm>
#include <iterator>
#include <thread>
#include "Random.h"
#include <queue> 
#include <iostream>

const int screenWidth = 800; //legacy
const int screenHeight = 600; //legacy

const int width = 800;
const int height = 600;

const float xSize = screenWidth/width; //legacy
const float ySize = screenHeight / height; //legacy
const float pi = std::acos(-1);


float flatten(float min, float max, float value) {
	if (value < min)return min;
	if (value > max)return max;
	return value;
}

struct midpoint {
	int left;
	int right;
	int randomness;
};

float squarePointDistance(sf::Vector2f pos1, sf::Vector2f pos2) {
	float a = abs(pos1.x - pos2.x);
	float b = abs(pos1.y - pos2.y);
	float c = a * a + b * b;
	return c;
}
Terrain::Terrain()
{
	
}


Terrain::~Terrain()
{
}



void Terrain::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.texture = &terrainTexture;
	//const sf::Vertex* ptr = horizontal_map_of_vertical_vertices[0][0];
    //std::vector<std::vector<sf::Vertex>> vertices;
	//target.draw(legacy_vertices, states);
	for (auto& it : horizontal_map_of_vertical_vertices) {
		const sf::Vertex* v = &it->at(0);
		target.draw(v, 600, sf::PrimitiveType::Points, states);
	}
}

Terrain::Terrain(const std::string & file) // file = image file for the texture
{
	horizontal_map_of_vertical_vertices = std::vector<std::vector<sf::Vertex>*>(799);
	terrainTexture.loadFromFile(file);
	terrainTexture.setRepeated(true);
	line_to_displace = std::vector<int>(width);
	hasFalling = false;

	randomize(100);
}

Terrain::Terrain(std::vector<std::vector<sf::Vertex>> neu) 
{

	
}


//void Terrain::ray_destroy_multi(sf::Vector2f origin, float angle, float other_angle, float thickness)
//{
//	//const float precision = 1.0f;
//	//sf::Vector2f cline = origin;
//	//for (int i = 0; i < round(thickness / 2); i++) {
//	//	cline.x = origin.x + 1 * i * cos(other_angle*pi / 180);
//	//	cline.y = origin.y + 1 * i * sin(other_angle*pi / 180);
//	//	while (cline.y >= 0 && cline.y < height && cline.x >= 0 && cline.x < width) {
//
//	//		//int x = round(cline.x) + round(cline.y)*(width);
//	//		int x = round(cline.x) + round(cline.y)*(width);
//	//		x = flatten(0, height*width - 1, x);
//	//		sf::Vertex* point = &vertices[x];
//	//		if (point->color.a != 0) {
//	//			point->color.a = 0;
//	//			range_destroy_single(round(cline.x), round(cline.y));
//	//		}
//	//		cline.x += precision * cos(angle*pi / 180);
//	//		cline.y += precision * sin(angle*pi / 180);
//	//	}
//	//}
//}

void Terrain::destroy_circle(sf::Vector2i pos, int radius) { 
	if (hasFalling) {
		//regenerate_terrain(terrain_to_fall);
	}
	int i = pos.x - radius;
	if (i < 0) i = 0;
	for (i; i < pos.x + radius && i < width; i++) {
		int j = pos.y - radius;
		if (j < 0) j = 0;
		for (j; j < pos.y + radius && j <= height; j++) {
			if (squarePointDistance(sf::Vector2f(pos.x,pos.y), sf::Vector2f(i,j)) < radius * radius) {
				new_destroy_single(i, j);
				terrain_to_fall.insert(i);
			}	
		}
	}

	hasFalling = true;
}

void Terrain::new_destroy_single(int x, int y) {

	if ((height - y) < horizontal_map_of_vertical_vertices[x]->size()) {
		if (line_to_displace[x] < y)
			line_to_displace[x] = y;
		//horizontal_map_of_vertical_vertices[x][height - y].color.a = 0;
		horizontal_map_of_vertical_vertices.at(x)->at(y).position.y = height - 1;
	}


	//auto& v = horizontal_map_of_vertical_vertices[x];
	//for (int i = 0; i < v.size(); i++) {
	//	if (v.at(i).position.y == y) {
	//		if(line_to_displace[x] < y)
	//			line_to_displace[x] = y;
	//		v.at(i).position = sf::Vector2f(x, height + 1);
	//	}
	//}
}

void Terrain::randomize(float roughness) { 
	line_to_displace.at(0) = rng::getRangeInt(100, 500);
	line_to_displace.at(width - 1) = rng::getRangeInt(100, 500);

	std::queue<midpoint> q; //queue

	midpoint m; 
	m.left = 0;
	m.right = width - 1;
	m.randomness = roughness;
	q.push(m);
	while (q.size() != 0) {
		midpoint n = q.front();
		q.pop();

		int center = (n.left + n.right) / 2;
		line_to_displace.at(center) = (line_to_displace.at(n.left) + line_to_displace.at(n.right)) / 2;
		line_to_displace.at(center) -= (n.randomness * -1, n.randomness);
		if (line_to_displace.at(center) >= 600) line_to_displace.at(center) = 599;
		if (line_to_displace.at(center) <0) line_to_displace.at(center) = 0;
		std::vector<sf::Vertex>* va = new std::vector<sf::Vertex>(height + 1, sf::Vertex(sf::Vector2f(center, height - 1)));

		for (int i = line_to_displace[center]; i < height; i++) {
			
			sf::Vertex v;
			//if (i >= line_to_displace[center]) {
				v.position = sf::Vector2f(center, i);
				v.texCoords = sf::Vector2f(center * 2, i * 2);
			//}
			//else v.position = sf::Vector2f(center + 1, height + 1);
			//vertices[(center + 1) * (i + 1)] = v;
				//legacy_vertices.append(v);
			va->at(i) = v; //v.push_back(v);
			
		}

		if (n.right - n.left > 2) {
			midpoint left;
			left.left = n.left;
			left.right = center;
			left.randomness = floor(n.randomness / 2);
			q.push(left);
			midpoint right;
			right.left = center;
			right.right = n.right;
			right.randomness = floor(n.randomness / 2);
			q.push(right);
		}

		//legacy_vertices.append(sf::Vertex(sf::Vector2f(center, height + 1)));
		//legacy_vertices.append(sf::Vertex(sf::Vector2f(center, line_to_displace[center])));

		horizontal_map_of_vertical_vertices[center] = va;
	}
	for (int i : line_to_displace) {
		//std::cout << i << std::endl;
	}
}

bool Terrain::contains_vertex_at(int i, int j) {
	int actual_height = horizontal_map_of_vertical_vertices[i]->at(j).position.y;
	if (actual_height < height - 1)
		return true;
	return false;


	//auto& v = horizontal_map_of_vertical_vertices[width];
	//for (int i = v->size() - 1; i > 0; i--) {
	//	if (v->at(i).position.y == height)
	//		return true;
	//}
	//return false;
}

int Terrain::get_top_vertex_position_of_vertical_array_at_width(int width) {
	return line_to_displace.at(width);
}

bool Terrain::contains_vertex_that_needs_to_fall(int width) {
	auto& v = horizontal_map_of_vertical_vertices[width];
	for (int i = v->size() - 1; i > 0; i--) {
		if ((v->at(i).position.y > height))
			return true;
	}
	return false;
}

void Terrain::fall_vertex(std::unordered_set<int> ttf) {
	for (auto& i : ttf) {
		for (int j = line_to_displace.at(i); j >= 0; j--) {
			sf::Vertex* v = &horizontal_map_of_vertical_vertices.at(i)->at(j);
			v->color = sf::Color::Red;
			if (v->position.y == line_to_displace.at(i)) {
				//ttf.erase(i);
				break;
			}
			if (v->position.y < 598) {

				horizontal_map_of_vertical_vertices.at(i)->at(j).position.y++;// = horizontal_map_of_vertical_vertices.at(i)->at(j + 1);
			}
		}
	}
}

void Terrain::regenerate_terrain(std::unordered_set<int> ttf) {

	for (int k : ttf) {
		int max = height;
		auto v = horizontal_map_of_vertical_vertices[k];
		for (auto& j : *v) {
			if (j.position.y < max)
				max = j.position.y;
		}
		line_to_displace.at(k) = max;
	}
}

void Terrain::logic() {

	if (hasFalling)
		fall_vertex(terrain_to_fall);
}