#ifndef RATRACING_MATERIAL_H
#define RATRACING_MATERIAL_H

#include "Hitable.h"
#include "Texture.h"

class Material
{
public:
	virtual bool scatter(const Ray& r, const HitRecord& record, vec3& attenuation, Ray& scattered) = 0;
	virtual vec3 emitted(float u, float  v, const vec3& p) const { return vec3(0, 0, 0); }
};

class Lambertian : public Material
{
private:
	vec3 albedo;
public:
	Lambertian() = delete;
	Lambertian(const vec3& a):albedo(a) {}
	bool scatter(const Ray& r, const HitRecord& record, vec3& attenuation, Ray& scattered) override;
	vec3 randomInUnitSphere();
};

class Metal : public Material
{
private:
	vec3 albedo;
	float fuzz;
public:
	Metal() = delete;
	Metal(const vec3& a, float f) :albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
	bool scatter(const Ray& r, const HitRecord& record, vec3& attenuation, Ray& scattered) override;
	vec3 reflect(const vec3& v, const vec3& n);
	vec3 randomInUnitSphere();
};

class Dielectric : public Material
{
private:
	float refIdx;
public:
	Dielectric() = delete;
	Dielectric(float r):refIdx(r) {}
	bool scatter(const Ray& r, const HitRecord& record, vec3& attenuation, Ray& scattered) override;
	bool refract(const vec3& v, const vec3& n, float nRatio, vec3& refracted);
	vec3 reflect(const vec3& v, const vec3& n);
	float schlick(float cos, float ref);
};

class DiffuseLight : public Material
{
public:
	Texture *emit;
	DiffuseLight(Texture* a) : emit(a) {}
	bool scatter(const Ray& rIn, const HitRecord& record, vec3& attenuation, Ray& scattered) override;
	vec3 emitted(float u, float v, const vec3& p) const override;
};

#endif

