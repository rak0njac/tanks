#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "consts.h"
#include "VerticalVertexArray.h"

class HorizontalVertexArray {
public:
	HorizontalVertexArray();
	~HorizontalVertexArray();
	void push(const int& position, const VerticalVertexArray& vec);
	void pop(const int& position);
private:
	std::vector<VerticalVertexArray*> vec;
};