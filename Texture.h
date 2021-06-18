#ifndef RAYTRACING_TEXTURE_H
#define RAYTRACING_TEXTURE_H

#include "vec3.h"

class Texture
{
public:
	virtual vec3 value(float u, float v, const vec3& p) const = 0;
};

class ConstantTexture : public Texture
{
public:
	vec3 color;
	ConstantTexture() = default;
	ConstantTexture(vec3 c) : color(c) {}
	vec3 value(float u, float v, const vec3& p) const { return color; }
};

#endif

