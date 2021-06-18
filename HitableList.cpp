//
// Created by vigorweijia on 2021/4/16.
//

#include "HitableList.h"

bool HitableList::HitObject(const Ray &r, float tMin, float tMax, HitRecord &record)
{
	HitRecord tempRecord;
	bool hitAnything = false;
	float closest = tMax;
	for (int i = 0; i < list_size; i++)
	{
		if (list[i]->HitObject(r, tMin, closest, tempRecord))
		{
			hitAnything = true;
			closest = tempRecord.t;
			record = tempRecord;
		}
	}
	return hitAnything;
}