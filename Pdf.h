#ifndef RAYTRACING_PDF_H
#define RAYTRACING_PDF_H

#include "vec3.h"
#include "Onb.h"
#include "utils.h"
#include "Hitable.h"

class Pdf
{
public:
	virtual float value(const vec3& direction) = 0;
	virtual vec3 generate() = 0;
};

class CosinePdf : public Pdf
{
public:
	Onb uvw;
	CosinePdf(const vec3& w) { uvw.buildFromW(w); }
	float value(const vec3& direction) override;
	vec3 generate() override;
};

class HitablePdf : public Pdf
{
private:
	vec3 o;
	Hitable* ptr;
public:
	HitablePdf(Hitable* p, const vec3& origin) : ptr(p), o(origin) { }
	float value(const vec3& direction) override;
	vec3 generate() override;
};

class MixturePdf : public Pdf
{
private:
	Pdf* p[2];
public:
	MixturePdf(Pdf* p0, Pdf* p1) { p[0] = p0; p[1] = p1; }
	float value(const vec3& direction) override;
	vec3 generate() override;
};

#endif

