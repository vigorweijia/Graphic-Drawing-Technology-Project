#include "Material.h"
#include "utils.h"

bool Lambertian::scatter(const Ray& r, const HitRecord& record, vec3& attenuation, Ray& scattered)
{
	vec3 target = record.p + record.normal + randomInUnitSphere();
	scattered = Ray(record.p, target - record.p);
	attenuation = albedo;
	return true;
}

vec3 Lambertian::randomInUnitSphere()
{
	vec3 p;
	do {
		p = 2.0 * vec3(randomUniform(), randomUniform(), randomUniform()) - vec3(1.0, 1.0, 1.0);
	} while (p.length() >= 1.0);
	return p;
}

bool Metal::scatter(const Ray& r, const HitRecord& record, vec3& attenuation, Ray& scattered)
{
	vec3 reflected = reflect(r.direction().unit(), record.normal);
	scattered = Ray(record.p, reflected + fuzz * randomInUnitSphere());
	attenuation = albedo;
	return dot(scattered.direction(), record.normal) > 0;
}

vec3 Metal::reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}

vec3 Metal::randomInUnitSphere()
{
	vec3 p;
	do {
		p = 2.0 * vec3(randomUniform(), randomUniform(), randomUniform()) - vec3(1.0, 1.0, 1.0);
	} while (p.length() >= 1.0);
	return p;
}

bool Dielectric::scatter(const Ray& r, const HitRecord& record, vec3& attenuation, Ray& scattered)
{
	float nRatio;
	vec3 outwardNormal;
	vec3 reflected = reflect(r.direction(), record.normal);
	vec3 refracted;
	attenuation = vec3(1.0, 1.0, 1.0);
	float cos;
	float reflectProb;
	if (dot(r.direction(), record.normal) > 0)
	{
		outwardNormal = negative(record.normal);
		nRatio = refIdx;
		cos = refIdx * dot(r.direction(), record.normal) / r.direction().length();
	}
	else
	{
		outwardNormal = record.normal;
		nRatio = 1.0 / refIdx;
		cos = -dot(r.direction(), record.normal) / r.direction().length();
	}
	if (refract(r.direction(), outwardNormal, nRatio, refracted))
	{
		reflectProb = schlick(cos, refIdx);
		// scattered = Ray(record.p, refracted);
	}
	else
	{
		reflectProb = 1.0;
		// scattered = Ray(record.p, reflected);
		// return false;
	}
	if (randomUniform() < reflectProb)
	{
		scattered = Ray(record.p, reflected);
	}
	else
	{
		scattered = Ray(record.p, refracted);
	}
	return true;
}

bool Dielectric::refract(const vec3& v, const vec3& n, float nRatio, vec3& refracted)
{
	vec3 uv = unit_vector(v);
	float dt = uv.dot(n);
	float discriminant = 1.0 - nRatio * nRatio * (1 - dt * dt);
	if (discriminant > 0)
	{
		refracted = nRatio * (uv - dt * n) - sqrt(discriminant) * n;
		return true;
	}
	else
	{
		return false;
	}
}

vec3 Dielectric::reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}

float Dielectric::schlick(float cos, float ref)
{
	float r0 = (1.0 - ref) / (1.0 + ref);
	r0 = r0 * r0;
	return r0 + (1.0 - r0)*pow(1 - cos, 5);
}