#include "Pdf.h"

float CosinePdf::value(const vec3& direction)
{
	float cosine = dot(unit_vector(direction), uvw.w());
	if (cosine > 0)
		return cosine / M_PI;
	else
		return 0;
}

vec3 CosinePdf::generate()
{
	return uvw.local(randomCosineDirection());
}

float HitablePdf::value(const vec3& direction)
{
	return ptr->PdfVal(o, direction);
}

vec3 HitablePdf::generate()
{
	return ptr->Random(o);
}

float MixturePdf::value(const vec3& direction)
{
	return 0.5*p[0]->value(direction) + 0.5*p[1]->value(direction);
}

vec3 MixturePdf::generate()
{
	if (randomUniform() < 0.5)
		return p[0]->generate();
	else
		return p[1]->generate();
}