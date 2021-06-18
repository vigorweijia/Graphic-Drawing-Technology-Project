//
// Created by vigorweijia on 2021/4/15.
//

#include "Sphere.h"

/*
 * dot( p(t)-C , p(t)-C ) = R^2
 * => dot( A+tB-C , A+tB-C ) = R^2
 * => t*t*dot(B,B) + 2*t*dot(B,A-C) + dot(A-C,A-C) - R^2 = 0
 */
bool Sphere::HitObject(const Ray &r, float tMin, float tMax, HitRecord &record)
{
	vec3 aMinusC = r.origin() - center;
	float a = r.direction().dot(r.direction());
	float b = 2 * r.direction().dot(aMinusC);
	float c = aMinusC.dot(aMinusC) - radius * radius;
	float delta = b * b - 4 * a * c;
	if (delta < 0)
		return false;

	float tempT = (-b - sqrt(delta)) / (2 * a);
	if (tMin < tempT && tempT < tMax)
	{
		record.t = tempT;
		record.p = r.loc_at_param(tempT);
		record.normal = (record.p - center) / radius;
		record.matPtr = matPtr;
		getSphereUV((record.p - center) / radius, record.u, record.v);
		return true;
	}
	tempT = (-b + sqrt(delta)) / (2 * a);
	if (tMin < tempT && tempT < tMax)
	{
		record.t = tempT;
		record.p = r.loc_at_param(tempT);
		record.normal = (record.p - center) / radius;
		record.matPtr = matPtr;
		getSphereUV((record.p - center) / radius, record.u, record.v);
		return true;
	}
	return false;
}

void Sphere::getSphereUV(const vec3&p, float& u, float& v)
{
	float phi = atan2(p.z(), p.x());
	float theta = asin(p.y());
	u = 1 - (phi + M_PI) / (2 * M_PI);
	v = (theta + M_PI / 2) / M_PI;
}

bool Plane::HitObject(const Ray &r, float tMin, float tMax, HitRecord &record)
{
	vec3 aMinusP0 = r.origin() - p0;
	float a = aMinusP0.dot(n);
	float b = r.direction().dot(n);
	if (fabs(b) < 0.000001) //parallel
		return false;

	float t = -a / b;
	if (tMin < t && t < tMax)
	{
		record.t = t;
		record.p = r.loc_at_param(t);
		record.normal = unit_vector(n);
		record.matPtr = matPtr;
		getPlaneUV(record.p, record.u, record.v);
		return true;
	}
	return false;
}

void Plane::getPlaneUV(const vec3& p, float& u, float& v)
{
	u = 0;
	v = 0;
}

bool Cylinder::HitObject(const Ray &r, float tMin, float tMax, HitRecord &record)
{
	vec3 B;
	vec3 A;
	vec3 C;
	float t1;
	float t2;
	switch (d)
	{
	case Cylinder::xAxis:
		A = vec3(0, r.origin().y(), r.origin().z());
		B = vec3(0, r.direction().y(), r.direction().z());
		C = vec3(0, center.y(), center.z());
		break;
	case Cylinder::yAxis:
		A = vec3(r.origin().x(), 0, r.origin().z());
		B = vec3(r.direction().x(), 0, r.direction().z());
		C = vec3(center.x(), 0, center.z());
		break;
	case Cylinder::zAxis:
		A = vec3(r.origin().x(), r.origin().y(), 0);
		B = vec3(r.direction().x(), r.direction().y(), 0);
		C = vec3(center.x(), center.y(), 0);
		break;
	default:
		break;
	}
	bool cross = Quadratic(dot(B,B), 2 * dot(B,A-C), dot(A-C,A-C)-radius*radius, t1, t2);

	vec3 p1 = r.loc_at_param(t1);
	vec3 p2 = r.loc_at_param(t2);
	bool p1valid = false;
	bool p2valid = false;

	switch (d)
	{
	case Cylinder::xAxis:
		if (center.x() - halfHeight < p1.x() && p1.x() < center.x() + halfHeight)
			p1valid = true;
		if (center.x() - halfHeight < p2.x() && p2.x() < center.x() + halfHeight)
			p2valid = true;
		break;
	case Cylinder::yAxis:
		if (center.y() - halfHeight < p1.y() && p1.y() < center.y() + halfHeight)
			p1valid = true;
		if (center.y() - halfHeight < p2.y() && p2.y() < center.y() + halfHeight)
			p2valid = true;
		break;
	case Cylinder::zAxis:
		if (center.z() - halfHeight < p1.z() && p1.z() < center.z() + halfHeight)
			p1valid = true;
		if (center.z() - halfHeight < p2.z() && p2.z() < center.z() + halfHeight)
			p2valid = true;
		break;
	default:
		break;
	}

	vec3 c1Center;
	vec3 n1;
	vec3 c2Center;
	vec3 n2;
	switch (d)
	{
	case Cylinder::xAxis:
		c1Center = center + vec3(halfHeight, 0, 0);
		n1 = vec3(1, 0, 0);
		c2Center = center + vec3(-halfHeight, 0, 0);
		n2 = vec3(-1, 0, 0);
		break;
	case Cylinder::yAxis:
		c1Center = center + vec3(0, halfHeight, 0);
		n1 = vec3(0, 1, 0);
		c2Center = center + vec3(0, -halfHeight, 0);
		n2 = vec3(0, -1, 0);
		break;
	case Cylinder::zAxis:
		c1Center = center + vec3(0, 0, halfHeight);
		n1 = vec3(0, 0, 1);
		c2Center = center + vec3(0, 0, -halfHeight);
		n2 = vec3(0, 0, -1);
		break;
	default:
		break;
	}
	float c1;
	float c2;
	bool circle1 = CrossCircle(r, c1Center, n1, c1);
	bool circle2 = CrossCircle(r, c2Center, n2, c2);

	bool hasSolution = false;
	float minT = tMax + 1;
	vec3 normal;
	vec3 p;
	if (p1valid && tMin < t1 && t1 < tMax)
	{
		hasSolution = true;
		if (t1 < minT)
		{
			minT = t1;
			p = r.loc_at_param(minT);
			switch (d)
			{
			case Cylinder::xAxis:
				normal = vec3(0, p.y() - center.y(), p.z() - center.z()) / radius;
				break;
			case Cylinder::yAxis:
				normal = vec3(p.x() - center.x(), 0, p.z() - center.z()) / radius;
				break;
			case Cylinder::zAxis:
				normal = vec3(p.x() - center.x(), p.y() - center.y(), 0) / radius;
				break;
			default:
				break;
			}
		}
	}
	if (p2valid && tMin < t2 && t2 < tMax)
	{
		hasSolution = true;
		if (t2 < minT)
		{
			minT = t2;
			p = r.loc_at_param(minT);
			switch (d)
			{
			case Cylinder::xAxis:
				normal = vec3(0, p.y() - center.y(), p.z() - center.z()) / radius;
				break;
			case Cylinder::yAxis:
				normal = vec3(p.x() - center.x(), 0, p.z() - center.z()) / radius;
				break;
			case Cylinder::zAxis:
				normal = vec3(p.x() - center.x(), p.y() - center.y(), 0) / radius;
				break;
			default:
				break;
			}
		}
	}
	if (circle1 && tMin < c1 && c1 < tMax)
	{
		hasSolution = true;
		if (c1 < minT)
		{
			minT = c1;
			p = r.loc_at_param(minT);
			normal = unit_vector(n1);
		}
	}
	if (circle2 && tMin < c2 && c2 < tMax)
	{
		hasSolution = true;
		if (c2 < minT)
		{
			minT = c2;
			p = r.loc_at_param(minT);
			normal = unit_vector(n2);
		}
	}
	if (hasSolution)
	{
		record.t = minT;
		record.p = p;
		record.normal = normal;
		record.matPtr = matPtr;
		return true;
	}
	return false;
}

bool Cylinder::Quadratic(float a, float b, float c, float& x1, float& x2)
{
	float delta = b * b - 4 * a * c;
	if (delta < 0)
	{
		return false;
	}

	x1 = (-b - sqrt(delta)) / (2 * a);
	x2 = (-b + sqrt(delta)) / (2 * a);
	return true;
}

bool Cylinder::CrossCircle(const Ray& r, const vec3& c, const vec3& n, float& t)
{
	vec3 aMinusP0 = r.origin() - c;
	float a = aMinusP0.dot(n);
	float b = r.direction().dot(n);
	if (fabs(b) < 0.000001) //parallel
		return false;

	float tempT = -a / b;
	vec3 p = r.loc_at_param(tempT);
	if (dot(p - c, p - c) > radius * radius)
		return false;
	t = tempT;
	return true;
}