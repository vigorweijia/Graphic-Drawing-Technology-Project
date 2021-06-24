#include "utils.h"

float randomUniform(float a, float b)
{
	return rand() * (b - a) / RAND_MAX;
}

vec3 randomCosineDirection()
{
	float r1 = randomUniform(0, 1.0);
	float r2 = randomUniform(0, 1.0);
	float z = sqrt(1.0 - r2);
	float phi = 2 * M_PI * r1;
	float x = cos(phi) * 2 * sqrt(r2);
	float y = sin(phi) * 2 * sqrt(r2);
	return vec3(x, y, z);
}