#pragma once
#include "Weekend.h"

struct hitRecord;

class Material
{
public:
	virtual bool scatter(
		const ray& r_in, const hitRecord& rec, color3& attenuation, ray& scattered
	) const = 0;
};

class Lambertian : public Material
{
public:
	color3 albedo;

	Lambertian(const color3& a) : albedo(a) {}

	virtual bool scatter(
		const ray& r_in, const hitRecord& rec, color3& attenuation, ray& scattered
	) const override
	{
		auto scatterDirection = rec.normal + randomUnitVector();
	}
};
