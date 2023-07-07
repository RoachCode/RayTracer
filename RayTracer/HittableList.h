#pragma once
#include "Hittable.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable
{
public:
	std::vector<shared_ptr<Hittable>> objects;

	HittableList() {}
	HittableList(shared_ptr<Hittable> object)
	{
		add(object);
	}
	void clear() { objects.clear(); }
	void add(shared_ptr<Hittable> object)
	{
		objects.push_back(object);
	}
	virtual bool hit(const ray& r, double tMin, double tMax, hitRecord& rec) const override;
};

bool HittableList::hit(const ray& r, double tMin, double tMax, hitRecord& rec) const
{
	hitRecord tempRec;
	bool hitAnything = false;
	auto closestSoFar = tMax;

	for (const auto& object : objects)
	{
		if (object->hit(r, tMin, closestSoFar, tempRec))
		{
			hitAnything = true;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
	}
	return hitAnything;
}
