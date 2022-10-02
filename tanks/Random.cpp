#include "Random.h"
#include <chrono>
namespace rnd {
	namespace {
        std::random_device dev;
        std::mt19937 rng(dev());
        //std::uniform_int_distribution<std::mt19937::result_type> dist(1,6); // distribution in range [1, 6]

        //std::cout << dist6(rnd) << std::endl;
	}

	int get_int(int x, int y) {
        std::uniform_int_distribution<std::mt19937::result_type> dist(x,y);
        return dist(rng);
	}

}