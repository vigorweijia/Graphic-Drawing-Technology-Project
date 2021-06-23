#ifndef RAYTRACING_AABB_H
#define RAYTRACING_AABB_H

#include "Ray.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class AABB
{
private:
	vec3 _min;
	vec3 _max;

public:
	AABB() = default;
	AABB(const vec3& a, const vec3& b) { _min = a; _max = b; }
	
	vec3 min() { return _min; }
	vec3 max() { return _max; }

	bool HitObject(const Ray& r, float tMin, float tMax);
};

AABB SurroundingBox(AABB box0, AABB box1);

#endif

