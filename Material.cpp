#include "Material.h"
#include "utils.h"

/*vec3 Material::randomCosineDirection()
{
	float r1 = randomUniform();
	float r2 = randomUniform();
	float z = sqrt(1.0 - r2);
	float phi = 2 * M_PI * r1;
	float x = cos(phi) * 2 * sqrt(r2);
	float y = sin(phi) * 2 * sqrt(r2);
	return vec3(x, y, z);
}*/

bool Lambertian::scatter(const Ray& r, const HitRecord& record, vec3& attenuation, Ray& scattered, float& pdf)
{
	/*vec3 target = record.p + record.normal + randomInUnitSphere();
	scattered = Ray(record.p, target - record.p);
	attenuation = albedo->value(record.u, record.v, record.p);
	pdf = dot(record.normal, scattered.direction()) / M_PI;
	return true;*/
	/*vec3 direction;
	do {
		direction = randomInUnitSphere();
	} while (dot(direction, record.normal) < 0);
	scattered = Ray(record.p, unit_vector(direction));
	attenuation = albedo->value(record.u, record.v, record.p);
	pdf = 0.5 / M_PI;
	return true;*/
	Onb uvw;
	uvw.buildFromW(record.normal);
	do {
		vec3 direction = uvw.local(randomCosineDirection());
		scattered = Ray(record.p, unit_vector(direction));
		pdf = dot(uvw.w(), scattered.direction()) / M_PI;
	} while (fabs(pdf) < 0.00001);
	attenuation = albedo->value(record.u, record.v, record.p);
	return true;
}

float Lambertian::scatterPdf(const Ray& r, const HitRecord& record, const Ray& scattered)
{
	float cosine = dot(record.normal, unit_vector(scattered.direction()));
	if (cosine < 0) cosine = 0;
	return cosine / M_PI;
}

vec3 Lambertian::randomInUnitSphere()
{
	vec3 p;
	do {
		p = 2.0 * vec3(randomUniform(), randomUniform(), randomUniform()) - vec3(1.0, 1.0, 1.0);
	} while (p.length() >= 1.0);
	return p;
}

bool Metal::scatter(const Ray& r, const HitRecord& record, vec3& attenuation, Ray& scattered, float& pdf)
{
	vec3 reflected = reflect(r.direction().unit(), record.normal);
	scattered = Ray(record.p, reflected + fuzz * randomInUnitSphere());
	attenuation = albedo;
	pdf = 0;
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

bool Dielectric::scatter(const Ray& r, const HitRecord& record, vec3& attenuation, Ray& scattered, float& pdf)
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

bool DiffuseLight::scatter(const Ray& rIn, const HitRecord& record, vec3& attenuation, Ray& scattered, float& pdf)
{
	return false;
}

vec3 DiffuseLight::emitted(const Ray& r, const HitRecord& record, float u, float v, const vec3& p) const
{
	if (dot(record.normal, r.direction()) < 0.0)
		return emit->value(u, v, p);
	else
		return vec3(0, 0, 0);
}

float Microfacet::ndf(const vec3& wo, const vec3& n)
{
	return (mAlpha + 2) / (2 * M_PI) * pow(dot(n, wo), mAlpha);
}

float Microfacet::scatterPdf(const Ray& r, const HitRecord& record, const Ray& scattered)
{
	float cosine = dot(record.normal, unit_vector(scattered.direction()));
	if (cosine < 0) 
		return 0;
	return cosine / M_PI;
}

bool Microfacet::scatter(const Ray& r, const HitRecord& record, vec3& attenuation, Ray& scattered, float& pdf)
{
	Onb uvw;
	uvw.buildFromW(record.normal);
	do {
		vec3 direction = uvw.local(randomCosineDirection());
		scattered = Ray(record.p, unit_vector(direction));
		pdf = ndf(scattered.direction(), record.normal);
	} while (fabs(pdf) < 0.00001);
	attenuation = evaluateColorResponse(record.normal, r.direction(), scattered.direction());
	return true;
}

vec3 Microfacet::evaluateColorResponse(const vec3& n, const vec3& wi, const vec3& wo)
{
	vec3 response =
		mKd * diffuseResponse() +
		(1 - mKd) *
		specularResponse(unit_vector(n), unit_vector(wi), unit_vector(wo));

	for (int i = 0; i < 3; i++)
		if (response[i] < 0.f) response[i] = 0.f;

	return response;
}

vec3 Microfacet::diffuseResponse()
{
	return mAlbedo / M_PI;
}

vec3 Microfacet::specularResponse(const vec3& n, const vec3& wi, const vec3& wo)
{
	vec3 wh = wi + wo;
	wh.make_unit();

	// Distribution
	float D =
		mAlpha * mAlpha /
		(M_PI * pow(1 + (mAlpha * mAlpha - 1) * pow(dot(n, wh), 2), 2));

	// Fresnel term
	vec3 F =
		mF0 + (vec3(1.0, 1.0, 1.0) - mF0) * pow(1 - ffmax(0, dot(wi, wh)), 5);
	// Geometric term
	float G = gSchlick(wi, n) * gSchlick(wo, n);

	float denom = (4. * dot(n, wi) * dot(n, wo));
	vec3 microfacetTerm = D * F * G / denom;

	return microfacetTerm;
}

float Microfacet::gSchlick(const vec3& w, const vec3& n) {
	float k = mAlpha * sqrt(2.0 / M_PI);
	return dot(n, w) / (dot(n, w) * (1 - k) + k);
}