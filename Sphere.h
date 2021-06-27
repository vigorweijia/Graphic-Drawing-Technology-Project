//
// Created by vigorweijia on 2021/4/15.
//

#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include <cmath>
#include <cassert>

#include "Hitable.h"
#include "HitableList.h"
#include "utils.h"
#include "Onb.h"

class Sphere: public Hitable{
private:
    float radius;
    vec3 center;
	Material* matPtr;
public:
    Sphere():center({0,0,0}),radius(1),matPtr(nullptr){}
	Sphere(const vec3& c, float _r, Material* p) { center = c; radius = _r; matPtr = p; }
    bool HitObject(const Ray &r, float tMin, float tMax, HitRecord &record) override;
	bool boundingBox(float t0, float t1, AABB& box) override;
	void getSphereUV(const vec3& p, float& u, float &v);
	float PdfVal(const vec3& o, const vec3& v) override;
	vec3 Random(const vec3& o) override;
	vec3 randomToSphere(float, float);
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
	bool boundingBox(float t0, float t1, AABB& box) override;
	void getPlaneUV(const vec3& p, float& u, float &v);
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
	bool boundingBox(float t0, float t1, AABB& box) override;
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

class XyRect : public Hitable {
public:
	float x0, x1, y0, y1, z;
	Material* matPtr;
	XyRect() = default;
	XyRect(float _x0, float _x1, float _y0, float _y1, float _z, Material* mat) :
		x0(_x0), x1(_x1), y0(_y0), y1(_y1), z(_z), matPtr(mat) {}
	bool HitObject(const Ray& r, float tMin, float tMax, HitRecord& record) override;
	bool boundingBox(float t0, float t1, AABB& box) override;
};

class XzRect : public Hitable {
public:
	float x0, x1, z0, z1, y;
	Material* matPtr;
	XzRect() = default;
	XzRect(float _x0, float _x1, float _z0, float _z1, float _y, Material* mat) :
		x0(_x0), x1(_x1), z0(_z0), z1(_z1), y(_y), matPtr(mat) {}
	bool HitObject(const Ray& r, float tMin, float tMax, HitRecord& record) override;
	bool boundingBox(float t0, float t1, AABB& box) override;
	float PdfVal(const vec3& o, const vec3& v) override;
	vec3 Random(const vec3& o) override;
};

class YzRect : public Hitable {
public:
	float y0, y1, z0, z1, x;
	Material* matPtr;
	YzRect() = default;
	YzRect(float _y0, float _y1, float _z0, float _z1, float _x, Material* mat) :
		y0(_y0), y1(_y1), z0(_z0), z1(_z1), x(_x), matPtr(mat) {}
	bool HitObject(const Ray& r, float tMin, float tMax, HitRecord& record) override;
	bool boundingBox(float t0, float t1, AABB& box) override;
};

class XzCircle : public Hitable{
public:
	float radius;
	vec3 center;
	Material* matPtr;
	XzCircle() = default;
	XzCircle(const vec3& c, float r, Material* mat) { center = c; radius = r; matPtr = mat; }
	bool HitObject(const Ray& r, float tMin, float tMax, HitRecord& record) override;
	bool boundingBox(float t0, float t1, AABB& box) override;
	float PdfVal(const vec3& o, const vec3& v) override;
	vec3 Random(const vec3& o) override;
};

class XzTriangle : public Hitable
{
public:
	vec3 A, B, C;
	Material* matPtr;
	XzTriangle() = delete;
	XzTriangle(const vec3& a, const vec3& b, const vec3& c, Material* mat) { A = a; B = b; C = c; matPtr = mat; }
	bool HitObject(const Ray& r, float tMin, float tMax, HitRecord& record) override;
	bool boundingBox(float t0, float t1, AABB& box) override;
	bool isInTriangle(const vec3& p);
	float PdfVal(const vec3& o, const vec3& v) override;
	vec3 Random(const vec3& o) override;
};

class FlipNormals : public Hitable {
public:
	Hitable* ptr;
	FlipNormals(Hitable *p) : ptr(p) {}
	bool HitObject(const Ray& r, float tMin, float tMax, HitRecord& record) override;
	bool boundingBox(float t0, float t1, AABB& box) override;
};

class Box : public Hitable {
public:
	vec3 pMin, pMax;
	Hitable* faceList;
	Box() = default;
	Box(const vec3& p0, const vec3& p1, Material* ptr);
	bool HitObject(const Ray& r, float tMin, float tMax, HitRecord& record) override;
	bool boundingBox(float t0, float t1, AABB& box) override;
};

class Translate : public Hitable {
public:
	Hitable* ptr;
	vec3 offset;
	Translate() = delete;
	Translate(Hitable* p, const vec3& displacement) : ptr(p), offset(displacement) {}
	bool HitObject(const Ray& r, float tMin, float tMax, HitRecord& record) override;
	bool boundingBox(float t0, float t1, AABB& box) override;
};

class RotateY : public Hitable {
public:
	Hitable* ptr;
	float sinTheta;
	float cosTheta;
	bool hasBox;
	AABB bbox;
	RotateY() = delete;
	RotateY(Hitable* p, float angle);
	bool HitObject(const Ray& r, float tMin, float tMax, HitRecord& record) override;
	bool boundingBox(float t0, float t1, AABB& box) override;
};

#endif //RAYTRACING_SPHERE_H
