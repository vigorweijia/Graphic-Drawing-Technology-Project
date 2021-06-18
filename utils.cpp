#include "utils.h"

float randomUniform(float a, float b)
{
	return rand() * (b - a) / RAND_MAX;
}