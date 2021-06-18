//
// Created by vigorweijia on 2021/4/15.
//

#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include <cmath>
#include <cassert>

#include "Hitable.h"

class Sphere: public Hitable{
private:
    float radius;
    vec3 center;
	Material* matPtr;
public:
    Sphere():center({0,0,0}),radius(1),matPtr(nullptr){}
	Sphere(const vec3& c, float _r, Material* p) { center = c; radius = _r; matPtr = p; }
    bool HitObject(const Ray &r, float tMin, float tMax, HitRecord &record) override;
};

class Plane: public Hitable{
private:
	vec3 n;
	vec3 p0;
	Material* matPtr;
public:
	Plane():p0({ 0,0,0 }), n({0,0,1}), matPtr(nullptr) {}
	Plane(const vec3& _p, const vec3& _n, Material* p) { p0 = _p; n = _n; matPtr = p; }
	bool HitObject(const Ray &r, float tMin, float tMax, HitRecord &record) override;
};

class Cylinder : public Hitable {
public:
	enum Dir
	{
		xAxis,
		yAxis,
		zAxis
	};
	Cylinder() :center({ 0,0,0 }), radius(1.0), height(1.0), d(yAxis), matPtr(nullptr) { halfHeight = 0.0; }
	Cylinder(const vec3& c, float r, float h, Dir _d, Material* p) { center = c; radius = r; height = h; d = _d; matPtr = p; halfHeight = height / 2.0; }
	bool HitObject(const Ray &r, float tMin, float tMax, HitRecord &record) override;
	bool Quadratic(float a, float b, float c, float& x1, float& x2);
	bool CrossCircle(const Ray& r, const vec3& c, const vec3& n, float& t);
private:
	Dir d;
	float halfHeight;
	float height;
	float radius;
	vec3 center;
	Material* matPtr;
};

#endif //RAYTRACING_SPHERE_H
