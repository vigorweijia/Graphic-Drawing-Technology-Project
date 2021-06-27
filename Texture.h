#ifndef RAYTRACING_TEXTURE_H
#define RAYTRACING_TEXTURE_H

#include "vec3.h"
#include "Noise.h"

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

class NoiseTexture : public Texture
{
public:
	NoiseTexture() {}
	vec3 value(float u, float v, const vec3& p) const 
	{ 
		float noiseColor = noise.getNoise(p);
		return vec3(1, 1, 1) * noiseColor; 
	}
	Noise noise;
};

class CheckerTexture : public Texture
{
public:
	Texture* even;
	Texture* odd;
	CheckerTexture() {}
	CheckerTexture(Texture* t0, Texture* t1) : even(t0), odd(t1) {}
	vec3 value(float u, float v, const vec3& p) const
	{
		float sines = sin(100 * p.x()) * sin(100 * p.y()) * sin(100 * p.z());
		if (sines < 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}
};

#endif

