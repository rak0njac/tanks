#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "Weapon.h"

namespace Parser {
	std::vector<Weapon> parse(std::string file);
}
