#pragma once
#include "Ray.h"
#include "Weekend.h"

class Material;

struct hitRecord
{
	point3 p;
	Vec3 normal;
	shared_ptr<Material> materialPtr;
	double t{};
	bool frontFace{false};

	inline void setFaceNormal(const ray& r, const Vec3& outwardNormal)
	{
		frontFace = dot(r.direction(), outwardNormal) < 0;
		// I have changed this. ref. listing 18. chapter 6.4.
		normal = frontFace ? outwardNormal : (- 1 * outwardNormal);
	}
};

class Hittable
{
public:
	virtual bool hit(const ray& r, double tMin, double tMax, hitRecord& rec) const = 0;
};
