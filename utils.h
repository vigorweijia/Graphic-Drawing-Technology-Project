#ifndef __UTILS_H__
#define __UTILS_H__

#include <cstdlib>

#include "vec3.h"

#define PI 3.14159265358979323846

float randomUniform(float a = 0.0, float b = 1.0);

vec3 randomCosineDirection();

float HammersleyPhi(int i);

#endif