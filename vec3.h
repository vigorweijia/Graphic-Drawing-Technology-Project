//
// Created by vigorweijia on 2021/4/13.
//

#ifndef RAYTRACING_VEC3_H
#define RAYTRACING_VEC3_H

#include <cmath>
#include <iostream>

#define M_PI 3.1415926

class vec3 {
public:
    float e[3]{};

    vec3() {e[0] = 0; e[1] = 0; e[2] = 0;}
    vec3(float a, float b, float c) {e[0] = a; e[1] = b; e[2] = c;}
    vec3(const vec3& v) {e[0] = v.e[0]; e[1] = v.e[1]; e[2] = v.e[2];}
    vec3(vec3&& v) noexcept {e[0] = v.e[0]; e[1] = v.e[1]; e[2] = v.e[2];}

    inline float x() const {return e[0];}
    inline float y() const {return e[1];}
    inline float z() const {return e[2];}

    inline const vec3& operator+() {return *this;}
    inline vec3 operator-() {return {-e[0], -e[1], -e[2]};}
    inline float operator[](int i ) const {return e[i];}
    inline float& operator[](int i) {return e[i];}

    vec3& operator=(const vec3&);
    vec3& operator+=(const vec3&);
    vec3& operator-=(const vec3&);
    vec3& operator*=(const vec3&);
    vec3& operator/=(const vec3&);
    vec3& operator*=(float);
    vec3& operator/=(float);
    bool operator ==(const vec3&);
    bool operator !=(const vec3&);
    vec3 operator+(const vec3&) const;
    vec3 operator-(const vec3&) const;
    vec3 operator*(const vec3&) const;
    vec3 operator/(const vec3&) const;
    vec3 operator*(float) const;
    vec3 operator/(float) const;
    friend vec3 operator*(float t, const vec3& v)
    {
        return {t*v.e[0], t*v.e[1], t*v.e[2]};
    }

    inline float length() const {return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);}
    void make_unit();
    vec3 unit();
    float dot(const vec3&);
    vec3 cross(const vec3&);
	vec3 negative();

    friend std::istream& operator>>(std::istream &is, vec3& v)
    {
        is >> v.e[0] >> v.e[1] >> v.e[2];
        return is;
    }
    friend std::ostream& operator<<(std::ostream &os, const vec3& v)
    {
        os << v.e[0] << " " << v.e[1] << " " << v.e[2];
        return os;
    }
};

vec3 unit_vector(const vec3& v);
float dot(const vec3& v1, const vec3& v2);
vec3 cross(const vec3& v1, const vec3& v2);
vec3 negative(const vec3& v);

#endif //RAYTRACING_VEC3_H
