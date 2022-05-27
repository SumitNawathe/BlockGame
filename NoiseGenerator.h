#ifndef NOISE_GENERATOR_H
#define NOISE_GENERATOR_H

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

#endif

