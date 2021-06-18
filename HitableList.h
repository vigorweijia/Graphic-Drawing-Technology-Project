//
// Created by vigorweijia on 2021/4/16.
//

#ifndef RAYTRACING_HITABLELIST_H
#define RAYTRACING_HITABLELIST_H

#include "Hitable.h"

class HitableList: public Hitable {
public:
    HitableList() = default;
    HitableList(Hitable **L, int n) {list = L; list_size = n;}
    bool HitObject(const Ray& r, float tMin, float tMax, HitRecord& record) override;
    Hitable **list{};
    int list_size{};
};

#endif //RAYTRACING_HITABLELIST_H
