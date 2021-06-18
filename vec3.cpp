//
// Created by vigorweijia on 2021/4/13.
//

#include "vec3.h"

vec3& vec3::operator=(const vec3& v){
    if(this == &v)
    {
       return *this;
    }
    e[0] = v.e[0];
    e[1] = v.e[1];
    e[2] = v.e[2];
    return *this;
}

vec3& vec3::operator+=(const vec3& v)
{
    *this = *this + v;
    return *this;
}

vec3& vec3::operator-=(const vec3& v)
{
    *this = *this - v;
    return *this;
}

vec3& vec3::operator*=(const vec3& v)
{
    *this = *this * v;
    return *this;
}

vec3& vec3::operator/=(const vec3& v)
{
    *this = *this / v;
    return *this;
}

vec3& vec3::operator*=(const float t)
{
    *this = *this * t;
    return *this;
}

vec3& vec3::operator/=(const float t)
{
    *this = *this / t;
    return *this;
}

bool vec3::operator==(const vec3& v)
{
    return e[0] == v.e[0] && e[1] == v.e[1] && e[2] == v.e[2];
}

bool vec3::operator!=(const vec3& v)
{
    return !(*this == v);
}

vec3 vec3::operator+(const vec3& v) const
{
    return {e[0]+v.e[0], e[1]+v.e[1], e[2]+v.e[2]};
}

vec3 vec3::operator-(const vec3& v) const
{
    return {e[0]-v.e[0], e[1]-v.e[1], e[2]-v.e[2]};
}

vec3 vec3::operator*(const vec3& v) const
{
    return {e[0]*v.e[0], e[1]*v.e[1], e[2]*v.e[2]};
}

vec3 vec3::operator/(const vec3& v) const
{
    return {e[0]/v.e[0], e[1]/v.e[1], e[2]/v.e[2]};
}

vec3 vec3::operator*(float t) const
{
    return {e[0]*t, e[1]*t, e[2]*t};
}

vec3 vec3::operator/(float t) const
{
    return {e[0]/t, e[1]/t, e[2]/t};
}

void vec3::make_unit()
{
    *this = *this / length();
}

vec3 vec3::unit()
{
    return *this / length();
}

float vec3::dot(const vec3& v)
{
    return e[0]*v.e[0] + e[1]*v.e[1] + e[2]*v.e[2];
}

vec3 vec3::cross(const vec3& v)
{
    return {e[1]*v.e[2]-e[2]*v.e[1],
            -e[0]*v.e[2]+e[2]*v.e[0],
            e[0]*v.e[1]-e[1]*v.e[0]};
}

vec3 vec3::negative()
{
	return { -e[0], -e[1], -e[2] };
}

vec3 unit_vector(const vec3& v)
{
    float len = v.length();
    return {v.e[0]/len, v.e[1]/len, v.e[2]/len};
}

float dot(const vec3& v1, const vec3& v2)
{
    return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1] + v1.e[2]*v2.e[2];
}

vec3 cross(const vec3& v1, const vec3& v2)
{
    return {v1.e[1]*v2.e[2]-v1.e[2]*v2.e[1],
            -v1.e[0]*v2.e[2]+v1.e[2]*v2.e[0],
            v1.e[0]*v2.e[1]-v1.e[1]*v2.e[0]};
}

vec3 negative(const vec3& v)
{
	return { -v.e[0], -v.e[1], -v.e[2] };
}