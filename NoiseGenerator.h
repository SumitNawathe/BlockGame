#pragma once

#include "PerlinNoise.hpp"

class NoiseGenerator {
public:
	static NoiseGenerator& getInstance() {
		static NoiseGenerator instance;
		return instance;
	}
	float generate2D(float x, float y);

private:
	NoiseGenerator();
	siv::BasicPerlinNoise<float> perlinNoise;
};

