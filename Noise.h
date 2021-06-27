#ifndef RAYTRACING_NOISE_H
#define RAYTRACING_NOISE_H

#include "vec3.h"
#include "utils.h"

static float* PerlinGenerate()
{
	float *p = new float[256];
	for (int i = 0; i < 256; i++)
		p[i] = randomUniform();
	return p;
}

static void permute(int* p, int n)
{
	for (int i = n - 1; i > 0; i--)
	{
		int target = int(randomUniform()*(i + 1));
		int tmp = p[i];
		p[i] = p[target];
		p[target] = tmp;
	}
	return;
}

static int* PerlinGeneratePerm()
{
	int* p = new int[256];
	for (int i = 0; i < 256; i++)
		p[i] = i;
	permute(p, 256);
	return p;
}

class Noise
{
public:
	Noise()
	{
		ranfloat = PerlinGenerate();
		perm_x = PerlinGeneratePerm();
		perm_y = PerlinGeneratePerm();
		perm_z = PerlinGeneratePerm();
	}
	float getNoise(const vec3& p) const
	{
		float u = p.x() - floor(p.x());
		float v = p.y() - floor(p.y());
		float w = p.z() - floor(p.z());
		int i = int(4 * p.x()) & 255;
		int j = int(4 * p.y()) & 255;
		int k = int(4 * p.z()) & 255;
		return ranfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
	}
	float *ranfloat;
	int *perm_x;
	int *perm_y;
	int *perm_z;
};

//float* Noise::ranfloat = PerlinGenerate();
//int* Noise::perm_x = PerlinGeneratePerm();
//int* Noise::perm_y = PerlinGeneratePerm();
//int* Noise::perm_z = PerlinGeneratePerm();

#endif

