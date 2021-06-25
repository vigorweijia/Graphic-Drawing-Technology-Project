//
// Created by vigorweijia on 2021/4/15.
//

#ifndef RAYTRACING_HITABLE_H
#define RAYTRACING_HITABLE_H

#include "Ray.h"
#include "AABB.h"

class Material;

struct HitRecord{
    float t;
    vec3 p;
    vec3 normal;
	Material* matPtr;
	//scale spherical coordinates (theta, phi) into u,v 
	float u;
	float v;
};

class Hitable {
public:
    virtual ~Hitable() = default;
    virtual bool HitObject(const Ray& r, float tMin, float tMax, HitRecord& record) = 0;
	virtual bool boundingBox(float t0, float t1, AABB& box) = 0;
	virtual float PdfVal(const vec3& o, const vec3& v) { return 0.0; }
	virtual vec3 Random(const vec3& o) { return vec3(1, 0, 0); }
};

#endif //RAYTRACING_HITABLE_H
