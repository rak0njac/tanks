#include "Random.h"
#include <chrono>
namespace rng {
	namespace {
		std::random_device rDev;
		std::mt19937 gen(rDev());
		//uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		//std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
		//gen.seed(ss);
		std::uniform_real_distribution<float> disF(0.0f, 0.0f);
		std::uniform_int_distribution<int> disI(0, 0);
	}
	float getRange(float x, float y) {
		disF.param(std::uniform_real_distribution<float>::param_type(x, y));
		return disF(gen);
	}
	int getRangeInt(int x, int y) {
		disI.param(std::uniform_int_distribution<int>::param_type(x, y));
		return disI(gen);
	}
	float zorandom() {
		disF.param(std::uniform_real_distribution<float>::param_type(0.f, 1.f));
		return disF(gen);
	}
}