#ifndef RATRACING_CAMERA_H
#define RATRACING_CAMERA_H

#include "Ray.h"
#include "utils.h"

#define PI 3.14159265358979323846

class Camera
{
private:
	vec3 origin;
	vec3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	float lenRadius;
public:
	Camera() = delete;
	Camera(const vec3& lookFrom, const vec3& lookAt, const vec3& vup, float vofv, float aspect, float aperture, float focusDist)
	{
		lenRadius = aperture / 2;
		float theta = vofv * PI / 180.0;
		float halfHeight = tan(theta / 2.0);
		float halfWidth = halfHeight * aspect;
		origin = lookFrom;
		w = unit_vector(lookFrom - lookAt);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		// lowerLeftCorner = vec3(-halfWidth, -halfHeight, -1.0);
		lowerLeftCorner = origin - halfWidth * focusDist * u - halfHeight * focusDist * v - focusDist * w;
		horizontal = 2.0 * halfWidth * focusDist * u;
		vertical = 2.0 * halfHeight * focusDist * v;
	}

	Ray getRay(float s, float t) 
	{
		vec3 rd = lenRadius * randomInUnitCricle();
		vec3 offset = u * rd.x() + v * rd.y();
		return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
	}

	vec3 randomInUnitCricle()
	{
		vec3 p;
		do {
			p = 2.0 * vec3(randomUniform(), randomUniform(), 0) - vec3(1.0, 1.0, 0);
		} while (p.length() >= 1.0);
		return p;
	}
};

#endif