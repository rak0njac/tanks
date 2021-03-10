#include "Terrain.h"
#include <math.h>
#include <algorithm>
#include <iterator>
#include <thread>
#include "Random.h"
#include <queue> 

const int screenWidth = 800;
const int screenHeight = 600;

const int width = 800;
const int height = 600;

const float xSize = screenWidth/width;
const float ySize = screenHeight / height;



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
	target.draw(vArray, states);
}

Terrain::Terrain(const std::string & file) // file = image file for the texture
{
	terrainTexture.loadFromFile(file);
	terrainTexture.setRepeated(true);
	//vArray.setPrimitiveType(sf::Quads);
	//vArray.setPrimitiveType(sf::Quads);
	vArray.resize(width * height); //4
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			//sf::Vertex* quad = &vArray[(i + j * width) * 4];
			sf::Vertex* point = &vArray[(i + j * width)];

			point->position = sf::Vector2f((i * xSize) +0.5f,(j * ySize) +0.5f);
			point->texCoords = sf::Vector2f(i * xSize*2, j * ySize*2);

			/*quad[0].position = sf::Vector2f(i * xSize, j * ySize);
			quad[1].position = sf::Vector2f((i + 1) * xSize, j* ySize);
			quad[2].position = sf::Vector2f((i + 1)* xSize, (j + 1)* ySize);
			quad[3].position = sf::Vector2f(i* xSize, (j + 1)* ySize);

			quad[0].texCoords = sf::Vector2f(i * xSize, j * ySize);
			quad[1].texCoords = sf::Vector2f((i + 1) * xSize, j* ySize);
			quad[2].texCoords = sf::Vector2f((i + 1)* xSize, (j + 1)* ySize);
			quad[3].texCoords = sf::Vector2f(i* xSize, (j + 1)* ySize);
			*/
		}
	}

	ranges.resize(width);
	for (int i = 0; i < ranges.size(); i++) {
		ranges[i].min = 0;
		ranges[i].max = height-1;
	}
	displacement(5.f, 200.f);
	applyRange();

}

void Terrain::destroy(sf::Vector2i pos, int radius) { //destroys ground in a circle at the given position with the given radius, currently only for debugging
	//int quadPos = pos.x + pos.y * screenWidth;
	//radius /= xSize * ySize;
	pos.x /= xSize;
	pos.y /= ySize;
	int i = pos.x - radius;
	if (i < 0) i = 0;
	for (i; i < pos.x + radius && i < width; i++) {
		int j = pos.y - radius;
		if (j < 0) j = 0;
		for (j; j < pos.y + radius && j < height; j++) {
			sf::Vertex* point = &vArray[(i + j * width)];
			if (point->color.a == 0) continue;

			if (squarePointDistance(sf::Vector2f(pos.x,pos.y), point->position) < radius * radius) {
				point->color.a = 0;
				Range* current = &ranges[i];
				while (true) {
					Range* next = current->next;
					int hight = height - 1 - j ;
					if (hight < current->max && hight > current->min) {
						int temp = current->max;
						current->max = hight - 1;
						Range* insert = new Range();
						insert->min = hight + 1;
						insert->max = temp;
						insert->next = current->next;
						current->next = insert;
						break;
					}
					if (hight == current->max) {
						current->max--;
						break;
					}
					if (hight == current->min && hight!=0) {
						current->min--;
						break;
					}
					if (!next) {
						break;
					}
					current = next;
				}
			}
			
		}
	}
	hasFalling = true;
}

void Terrain::displacement(float displace, float roughness) { //1D midpoint displacement for generating random terrain
	//TODO: NEEDS TO BE FIXED, CRASHES ONCE IN A BLUE MOON
	ranges[0].max = rng::getRangeInt(100,500);
	ranges[width-1].max = rng::getRangeInt(100, 500);
	std::queue<midpoint> q;
	midpoint m;
	m.left = 0;
	m.right = width - 1;
	m.randomness = roughness;
	q.push(m);
	while (q.size() != 0) {
		midpoint n = q.front();
		q.pop();
		int center = (n.left + n.right) / 2;

		ranges[center].max = (ranges[n.left].max + ranges[n.right].max) / 2;
		ranges[center].max += rng::getRangeInt(-n.randomness, n.randomness);
		if (ranges[center].max >= 600) ranges[center].max = 599;
		if (ranges[center].max <0) ranges[center].max = 0;

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
	}



}

void Terrain::applyRange() { //makes pixels that aren't in the current range transparent
	for (int i = 0; i < ranges.size(); i++) {
		for (int j = ranges[i].max; j <= height; j++) {
			int y = i + ((height - (j-1))*width);
			if (y < 0) y = 0;
			if (y >= width * height) y = width * height - 1;
			vArray[y].color.a = 0;
		}
	}
}

void Terrain::groundFall() { //for multithreading ground
	std::thread th1(&Terrain::groundFallThreaded,this, 0);
	std::thread th2(&Terrain::groundFallThreaded, this, 1);
	std::thread th3(&Terrain::groundFallThreaded, this, 2);
	std::thread th4(&Terrain::groundFallThreaded, this, 3);
	std::thread th5(&Terrain::groundFallThreaded, this, 4);
	std::thread th6(&Terrain::groundFallThreaded, this, 5);
	std::thread th7(&Terrain::groundFallThreaded, this, 6);
	std::thread th8(&Terrain::groundFallThreaded, this, 7);
	std::thread th9(&Terrain::groundFallThreaded, this, 8);
	std::thread th10(&Terrain::groundFallThreaded, this, 9);
	std::thread th11(&Terrain::groundFallThreaded, this, 10);
	std::thread th12(&Terrain::groundFallThreaded, this, 11);

	th1.join();
	th2.join();
	th3.join();
	th4.join();
	th5.join();
	th6.join();
	th7.join();
	th8.join();
	th9.join();
	th10.join();
	th11.join();
	th12.join();
}

void Terrain::groundFallThreaded(int thread) {//makes each row of ground fall down 1 pixel
	if (!hasFalling) return;

	for (int i = thread; i < ranges.size(); i+=12) {
		Range* current = &ranges[i];
		Range* prev = nullptr;
		while (true) {
			if (!current) break;

			Range* next = current->next;
			if (current->min != 0) {
				for (int j = current->min; j <= current->max; j++) {
					int y = height - (j+1);
					if (y >= 599 || y<0) continue;
					//std::swap(vArray[(i + y * width)], vArray[(i + (y + 1) * width)]);
					/*sf::Vector2f temp = std::move(vArray[(i + y * width)].texCoords);
					vArray[(i + y * width)].texCoords = std::move(vArray[(i + (y + 1) * width)].texCoords);
					vArray[(i + (y + 1) * width)].texCoords = std::move(temp);*/
					sf::Vertex& falling = vArray[(i + y * width)];
					sf::Vertex& rising = vArray[(i + (y+1) * width)];
					float x1 = falling.texCoords.x;
					float y1 = falling.texCoords.y;
					falling.texCoords.x = rising.texCoords.x;
					falling.texCoords.y = rising.texCoords.y;
					rising.texCoords.x = x1;
					rising.texCoords.y = y1;

					falling.color.a = 0;
					rising.color.a = 255;
				}
				current->min--;
				current->max--;
			}

			if (prev && current->min <= prev->max) {
				prev->max = current->max+1;
				prev->next = current->next;
				delete current;
				current = prev->next;
				continue;
			}
			prev = current;
			current = next;
		}
	}
}

Range * Terrain::operator[](int i)
{
	if (i < 0 || i >= width)
		return nullptr;
	return &ranges[i];
}

Range * Terrain::operator[](float i)
{
	return (*this)[static_cast<int>(round(i))];
}


