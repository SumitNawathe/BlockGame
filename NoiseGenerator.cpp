#include "NoiseGenerator.h"

#include <ctime>

NoiseGenerator::NoiseGenerator() {
	perlinNoise.reseed((unsigned int)time(0));
}

float NoiseGenerator::generate2D(float x, float y) {
	float n_2048 = perlinNoise.normalizedOctave2D_01(x / 2048.0f, y / 2048.0f, 4);

	float a = perlinNoise.normalizedOctave2D_01(x / 128.0f, y / 128.0f, 4);
	float b = perlinNoise.normalizedOctave2D_01(x / 64.0f, y / 64.0f, 4);
	float c = perlinNoise.normalizedOctave2D_01(x / 32.0f, y / 32.0f, 4);
	float d = perlinNoise.normalizedOctave2D_01(x / 16.0f, y / 16.0f, 4);
	float e = perlinNoise.normalizedOctave2D_01(x / 8.0f, y / 8.0f, 4);
	float f = perlinNoise.normalizedOctave2D_01(x / 4.0f, y / 4.0f, 4);
	float g = perlinNoise.normalizedOctave2D_01(x / 2.0f, y / 2.0f, 4);
	float h = perlinNoise.normalizedOctave2D_01(x, y, 4);
	float i = perlinNoise.normalizedOctave2D_01(x * 2.0f, y * 2.0f, 4);
	return (512*n_2048 + 128 * a + 32 * b + 32 * c + 8 * d + 8 * e + 8 * f) / (512 + 128 + 32 + 32 + 8 + 8 + 8);
}

