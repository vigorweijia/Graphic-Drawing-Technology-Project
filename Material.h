#ifndef RATRACING_MATERIAL_H
#define RATRACING_MATERIAL_H

#include "Hitable.h"
#include "Texture.h"
#include "Onb.h"

class Material
{
public:
	bool isSpecular;
	virtual bool scatter(const Ray& r, const HitRecord& record, vec3& attenuation, Ray& scattered, float& pdf) = 0;
	virtual float scatterPdf(const Ray& r, const HitRecord& record, const Ray& scattered) { return 0.0; }
	virtual vec3 emitted(const Ray& r, const HitRecord& record, float u, float  v, const vec3& p) const { return vec3(0, 0, 0); }
	//vec3 randomCosineDirection();
};

class Lambertian : public Material
{
private:
	Texture* albedo;
public:
	Lambertian() = delete;
	Lambertian(Texture* a) :albedo(a) { isSpecular = false; }
	bool scatter(const Ray& r, const HitRecord& record, vec3& attenuation, Ray& scattered, float& pdf) override;
	float scatterPdf(const Ray& r, const HitRecord& record, const Ray& scattered) override;
	vec3 randomInUnitSphere();
};

class Metal : public Material
{
private:
	vec3 albedo;
	float fuzz;
public:
	Metal() = delete;
	Metal(const vec3& a, float f) :albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; isSpecular = true; }
	bool scatter(const Ray& r, const HitRecord& record, vec3& attenuation, Ray& scattered, float& pdf) override;
	vec3 reflect(const vec3& v, const vec3& n);
	vec3 randomInUnitSphere();
};

class Dielectric : public Material
{
private:
	float refIdx;
public:
	Dielectric() = delete;
	Dielectric(float r) :refIdx(r) { isSpecular = true; }
	bool scatter(const Ray& r, const HitRecord& record, vec3& attenuation, Ray& scattered, float& pdf) override;
	bool refract(const vec3& v, const vec3& n, float nRatio, vec3& refracted);
	vec3 reflect(const vec3& v, const vec3& n);
	float schlick(float cos, float ref);
};

class DiffuseLight : public Material
{
public:
	Texture *emit;
	DiffuseLight() = delete;
	DiffuseLight(Texture* a) : emit(a) { isSpecular = true; }
	bool scatter(const Ray& rIn, const HitRecord& record, vec3& attenuation, Ray& scattered, float& pdf) override;
	vec3 emitted(const Ray& r, const HitRecord& record, float u, float v, const vec3& p) const override;
};

class Microfacet : public Material
{
private:
	float mKd;
	vec3 mAlbedo;
	float mAlpha;
	vec3 mF0;
public:
	Microfacet()
	{
		mKd = M_PI;
		mAlbedo = vec3(0.1, 0.1, 0.9);
		mAlpha = 0.5;
		mF0 = vec3(0.31, 0.31, 0.31);
		isSpecular = true;
	}
	Microfacet(float kd, float alpha, const vec3& albedo, const vec3 F0)
	{
		mKd = kd;
		mAlbedo = albedo;
		mAlpha = alpha;
		mF0 = F0;
		isSpecular = true;
	}
	bool scatter(const Ray& rIn, const HitRecord& record, vec3& attenuation, Ray& Scattered, float& pdf) override;
	vec3 reflect(const vec3& v, const vec3& n);
	vec3 evaluateColorResponse(const vec3& n, const vec3& wi, const vec3& wo);
	vec3 diffuseResponse();
	vec3 specularResponse(const vec3& n, const vec3& wi, const vec3& wo);
	float gSchlick(const vec3& w, const vec3& n);
	float ndf(const vec3& wo, const vec3& n);
	float scatterPdf(const Ray& r, const HitRecord& record, const Ray& scattered) override;
};

#endif

