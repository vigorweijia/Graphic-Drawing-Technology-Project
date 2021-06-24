#ifndef RAYTRACING_ONB_H
#define RAYTRACING_ONB_H

#include "vec3.h"

class Onb
{
public:
	vec3 axis[3];
	Onb() {}
	inline vec3 operator[](int i) const { return axis[i]; }
	vec3 u() const { return axis[0]; }
	vec3 v() const { return axis[1]; }
	vec3 w() const { return axis[2]; }
	vec3 local(float a, float b, float c) { return a * u() + b * v() + c * w(); }
	vec3 local(const vec3& a) { return local(a.x(), a.y(), a.z()); }
	void buildFromW(const vec3& n);
};

#endif

