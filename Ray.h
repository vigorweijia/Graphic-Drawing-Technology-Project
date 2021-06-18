//
// Created by vigorweijia on 2021/4/15.
//

#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H

#include "vec3.h"

class Ray {
private:
    vec3 from;
    vec3 d;
public:
    Ray():from({0,0,0}),d({0,0,0}) {}
    Ray(const vec3& a, const vec3& b) {from = a; d = b;}
    vec3 loc_at_param(float t) const {return from + t * d;}
    vec3 origin() const {return from;}
    vec3 direction() const {return d;}
};


#endif //RAYTRACING_RAY_H
