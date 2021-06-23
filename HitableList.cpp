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

bool HitableList::boundingBox(float t0, float t1, AABB& box)
{
	if (list_size < 1) return false;
	AABB tempBox;
	bool firstTrue = list[0]->boundingBox(t0, t1, tempBox);
	if (!firstTrue)
		return false;
	for (int i = 1; i < list_size; i++)
	{
		if (list[i]->boundingBox(t0, t1, tempBox))
		{
			box = SurroundingBox(box, tempBox);
		}
		else
		{
			return false;
		}
	}
	return true;
}