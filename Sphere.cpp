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

bool Sphere::boundingBox(float t0, float t1, AABB& box)
{
	box = AABB(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
	return true;
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

bool Plane::boundingBox(float t0, float t1, AABB& box)
{
	return false;
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

bool Cylinder::boundingBox(float t0, float t1, AABB& box)
{
	return false;
}

bool XyRect::HitObject(const Ray& r, float tMin, float tMax, HitRecord& record)
{
	float t = (z - r.origin().z()) / r.direction().z();
	if (t < tMin || t > tMax) 
		return false;
	float x = r.origin().x() + t * r.direction().x();
	float y = r.origin().y() + t * r.direction().y();
	if (x < x0 || x > x1 || y < y0 || y > y1)
		return false;
	record.u = (x - x0) / (x1 - x0);
	record.v = (y - y0) / (y1 - y0);
	record.t = t;
	record.matPtr = matPtr;
	record.p = r.loc_at_param(t);
	record.normal = vec3(0, 0, 1);
	return true;
}

bool XyRect::boundingBox(float t0, float t1, AABB& box)
{
	box = AABB(vec3(x0, y0, z - 0.0001), vec3(x1, y1, z + 0.0001));
	return true;
}

bool XzRect::HitObject(const Ray& r, float tMin, float tMax, HitRecord& record)
{
	float t = (y - r.origin().y()) / r.direction().y();
	if (t < tMin || t > tMax)
		return false;
	float x = r.origin().x() + t * r.direction().x();
	float z = r.origin().z() + t * r.direction().z();
	if (x < x0 || x > x1 || z < z0 || z > z1)
		return false;
	record.u = (x - x0) / (x1 - x0);
	record.v = (z - z0) / (z1 - z0);
	record.t = t;
	record.matPtr = matPtr;
	record.p = r.loc_at_param(t);
	record.normal = vec3(0, 1, 0);
	return true;
}

bool XzRect::boundingBox(float t0, float t1, AABB& box)
{
	box = AABB(vec3(x0, y - 0.0001, z0), vec3(x1, y + 0.0001, z1));
	return true;
}

float XzRect::PdfVal(const vec3& o, const vec3& v)
{
	HitRecord rec;
	if (this->HitObject(Ray(o, v), 0.001, 1000, rec))
	{
		float area = (x1 - x0) * (z1 - z0);
		float distanceSquared = rec.t * rec.t * v.length() * v.length();
		float cosine = fabs(dot(v, rec.normal)) / v.length();
		return distanceSquared / (cosine * area);
	}
	else
		return 0;
}

vec3 XzRect::Random(const vec3& o)
{
	vec3 randomPoint = vec3(x0 + randomUniform()*(x1 - x0), y, z0 + randomUniform()*(z1 - z0));
	return randomPoint - o;
}

bool YzRect::HitObject(const Ray& r, float tMin, float tMax, HitRecord& record)
{
	float t = (x - r.origin().x()) / r.direction().x();
	if (t < tMin || t > tMax)
		return false;
	float y = r.origin().y() + t * r.direction().y();
	float z = r.origin().z() + t * r.direction().z();
	if (y < y0 || y > y1 || z < z0 || z > z1)
		return false;
	record.u = (y - y0) / (y1 - y0);
	record.v = (z - z0) / (z1 - z0);
	record.t = t;
	record.matPtr = matPtr;
	record.p = r.loc_at_param(t);
	record.normal = vec3(1, 0, 0);
	return true;
}

bool YzRect::boundingBox(float t0, float t1, AABB& box)
{
	box = AABB(vec3(x - 0.0001, y0, z0), vec3(x + 0.0001, y1, z1));
	return true;
}

bool FlipNormals::HitObject(const Ray& r, float tMin, float tMax, HitRecord& record)
{
	if (ptr->HitObject(r, tMin, tMax, record))
	{
		record.normal = -record.normal;
		return true;
	}
	else
	{
		return false;
	}
}

bool FlipNormals::boundingBox(float t0, float t1, AABB& box)
{
	return ptr->boundingBox(t0, t1, box);
}

Box::Box(const vec3& p0, const vec3& p1, Material* ptr)
{
	pMin = p0;
	pMax = p1;
	Hitable** list = new Hitable*[6];
	list[0] = new XyRect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr);
	list[1] = new FlipNormals(new XyRect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));
	list[2] = new XzRect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr);
	list[3] = new FlipNormals(new XzRect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));
	list[4] = new YzRect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr);
	list[5] = new FlipNormals(new YzRect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
	faceList = new HitableList(list, 6);
}

bool Box::HitObject(const Ray& r, float tMin, float tMax, HitRecord& record)
{
	return faceList->HitObject(r, tMin, tMax, record);
}

bool Box::boundingBox(float t0, float t1, AABB& box)
{
	box = AABB(pMin, pMax);
	return true;
}

bool Translate::HitObject(const Ray& r, float tMin, float tMax, HitRecord& record)
{
	Ray movedRay(r.origin() - offset, r.direction());
	if (ptr->HitObject(movedRay, tMin, tMax, record))
	{
		record.p += offset;
		return true;
	}
	else
	{
		return false;
	}
}

bool Translate::boundingBox(float t0, float t1, AABB& box)
{
	if (ptr->boundingBox(t0, t1, box))
	{
		box = AABB(box.min() + offset, box.max() + offset);
		return true;
	}
	else
	{
		return false;
	}
}

RotateY::RotateY(Hitable* p, float angle)
{
	float radians = (M_PI / 180.0) * angle;
	sinTheta = sin(radians);
	cosTheta = cos(radians);
	ptr = p;

	hasBox = ptr->boundingBox(0, 1, bbox);
	vec3 minn(FLT_MAX, FLT_MAX, FLT_MAX);
	vec3 maxn(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
			{
				float x = i * bbox.max().x() + (1 - i) * bbox.min().x();
				float y = j * bbox.max().y() + (1 - j) * bbox.min().y();
				float z = k * bbox.max().z() + (1 - k) * bbox.min().z();
				float newx = cosTheta * x + sinTheta * z;
				float newz = -sinTheta * x + cosTheta * z;
				vec3 tester(newx, y, newz);
				for (int c = 0; c < 3; c++)
				{
					if (tester[c] > maxn[c])
						maxn[c] = tester[c];
					if (tester[c] < minn[c])
						minn[c] = tester[c];
				}
			}
	bbox = AABB(minn, maxn);
}

bool RotateY::HitObject(const Ray& r, float tMin, float tMax, HitRecord& record)
{
	vec3 origin = r.origin();
	vec3 direction = r.direction();
	origin[0] = cosTheta * r.origin()[0] - sinTheta * r.origin()[2];
	origin[2] = sinTheta * r.origin()[0] + cosTheta * r.origin()[2];
	direction[0] = cosTheta * r.direction()[0] - sinTheta * r.direction()[2];
	direction[2] = sinTheta * r.direction()[0] + cosTheta * r.direction()[2];
	Ray rotatedRay(origin, direction);
	if (ptr->HitObject(rotatedRay, tMin, tMax, record))
	{
		vec3 p = record.p;
		vec3 normal = record.normal;
		p[0] = cosTheta * record.p[0] + sinTheta * record.p[2];
		p[2] = -sinTheta * record.p[0] + cosTheta * record.p[2];
		normal[0] = cosTheta * record.normal[0] + sinTheta * record.normal[2];
		normal[2] = -sinTheta * record.normal[0] + cosTheta * record.normal[2];
		record.p = p;
		record.normal = normal;
		return true;
	}
	else
	{
		return false;
	}
}

bool RotateY::boundingBox(float t0, float t1, AABB& box)
{
	box = bbox;
	return hasBox;
}