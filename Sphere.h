#pragma once
#include "Hittable.h"
#include "Vec3.h"

class Sphere : public Hittable
{
public:
	point3 center;
	double radius{};
	shared_ptr<Material> materialPtr;

	Sphere() {}
	Sphere(point3 cen, double r, shared_ptr<Material> m) 
		: center(cen), radius(r), materialPtr(m) {};

	virtual bool hit(const ray& r, double tMin, double tMax, hitRecord& rec) const override;
};

bool Sphere::hit(const ray& r, double tMin, double tMax, hitRecord& rec) const
{
	Vec3 oc = r.origin() - center;
	auto a = r.direction().lengthSquared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.lengthSquared() - radius * radius;

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) { return false; }
	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtd) / a;
	if (root < tMin || tMax < root)
	{
		root = (-half_b + sqrtd) / a;
		if (root < tMin || tMax < root) { return false; }
	}

	rec.t = root;
	rec.p = r.at(rec.t);
	Vec3 outwardNormal = (rec.p - center) / radius;
	rec.setFaceNormal(r, outwardNormal);
	rec.materialPtr = materialPtr;

	return true;
}
