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

 sf::VertexArray genMesh(int mRadius, int mCenterX, int mCenterY) {
	 int segments = mRadius * 4;
	 float step = 6.283185f / segments;
	float angle = 0.0f;
	sf::VertexArray vertices;

	vertices.resize(segments);
	for (int i = 0; i < segments; ++i, angle += step) {
		float vertX = mCenterX + cosf(angle) * mRadius;
		float vertY = mCenterY + sinf(angle) * mRadius;
		auto vert = sf::Vertex(sf::Vector2f(vertX, vertY));
		vert.color = sf::Color::Red;
		vertices[i] = vert;
	}

	return vertices;
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



	


	for (auto& it : horizontal_map_of_vertical_vertices) {
		const sf::Vertex* v = &it->at(0);
		target.draw(v, 600, sf::PrimitiveType::Points, states);
	}





	sf::VertexArray va;
	for (int i = 0; i < boundaries.size(); i++) {
		sf::Vertex v(sf::Vector2f(i, boundaries[i]));
		v.color = sf::Color::Red;
		va.append(v);
	}

	//target.draw(vaglobal);
	target.draw(va);
	target.draw(genMesh(25, 400, 200));
}

Terrain::Terrain(const std::string & file) // file = image file for the texture
{
	horizontal_map_of_vertical_vertices = std::vector<std::vector<sf::Vertex>*>(799);
	terrainTexture.loadFromFile(file);
	terrainTexture.setRepeated(true);
	boundaries = std::vector<int>(width);
	hasFalling = false;

	randomize(100);




	 
	//vaglobal.resize(801 * 601);
}

Terrain::Terrain(std::vector<std::vector<sf::Vertex>> neu) 
{

	
}


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
	if (boundaries[x] < y)
		boundaries[x] = y;

	//horizontal_map_of_vertical_vertices.at(x)->at(y).position.y = height - 1;  // bugs

	for (auto& v : *horizontal_map_of_vertical_vertices.at(x)) {   //works correctly
		if (v.position.y == y)
			v.position.y = height - 1;
	}
}

void Terrain::randomize(float roughness) { 
	boundaries.at(0) = rng::getRangeInt(100, 500);
	boundaries.at(width - 1) = rng::getRangeInt(100, 500);

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
		boundaries.at(center) = (boundaries.at(n.left) + boundaries.at(n.right)) / 2;
		boundaries.at(center) -= (n.randomness * -1, n.randomness);
		if (boundaries.at(center) >= 600) boundaries.at(center) = 599;
		if (boundaries.at(center) <0) boundaries.at(center) = 0;
		std::vector<sf::Vertex>* va = new std::vector<sf::Vertex>(height + 1, sf::Vertex(sf::Vector2f(center, height - 1)));

		for (int i = boundaries[center]; i < height; i++) {
			
			sf::Vertex v;
			//if (i >= boundaries[center]) {
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
		//legacy_vertices.append(sf::Vertex(sf::Vector2f(center, boundaries[center])));

		horizontal_map_of_vertical_vertices[center] = va;
	}
	for (int i : boundaries) {
		//std::cout << i << std::endl;
	}
}

bool Terrain::contains_vertex_at(int i, int j) {
	if (boundaries[i] < j)
		return true;
	return false;
}

int Terrain::get_top_vertex_position_of_vertical_array_at_width(int width) {
	return boundaries.at(width);
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
	//std::cout << ttf.size() << std::endl;
	static std::unordered_set<int> temp;

	if (ttf.empty()) {
		regenerate_terrain(temp);
		temp.clear();
		hasFalling = false;
		return;
	}

	if (temp.empty()) {
		temp = ttf;
	}

	//regenerate_terrain(temp);

	for (auto& i : ttf) {

		std::vector <sf::Vertex*> vert_array;
		for (int j = boundaries.at(i) - 1; j >= 0; j--) {
			sf::Vertex* v = &horizontal_map_of_vertical_vertices.at(i)->at(j);
			//v->color = sf::Color::Red;
			if (v->position.y < height - 1) {
				vert_array.push_back(v);
			}
		}

		if (vert_array.empty()) {
			terrain_to_fall.erase(i);
			continue;
		}

		for (auto& v : vert_array) {

			if (v->position.y >= boundaries.at(i) && v->position.y < height - 1) {
				terrain_to_fall.erase(i);
				boundaries.at(i) = vert_array.at(vert_array.size() - 1)->position.y;
				vert_array.clear();
				break;
			}
			v->position.y++;
		}
	}
}

void Terrain::regenerate_terrain(std::unordered_set<int> t) {
	for (auto& i : t) {
		for (int j = height; j >= boundaries.at(i); j--) {
			horizontal_map_of_vertical_vertices.at(i)->at(j).position.y = j;
		}
	}
}

void Terrain::logic() {

	if (hasFalling)
		fall_vertex(terrain_to_fall);


	//sf::VertexArray valocal;
	//valocal.resize(801 * 601);
	//int count = 0;
	//for (int i = 0; i < horizontal_map_of_vertical_vertices.size(); i++) {
	//	for (int j = 0; j < horizontal_map_of_vertical_vertices.at(i)->size(); j++, count++) {
	//		valocal[count] = horizontal_map_of_vertical_vertices.at(i)->at(j);
	//	}
	//}
	//vaglobal = valocal;
	//valocal.clear();
}
