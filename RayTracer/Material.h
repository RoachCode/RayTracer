#pragma once
#include "Weekend.h"
/*
class Material;

struct hitRecord
{
	point3 p;
	Vec3 normal;
	shared_ptr<class Material> matPtr;
	double t;
	bool frontFace;

	inline void setFaceNormal(const ray& r, const Vec3& outwardNormal)
	{
		frontFace = dot(r.direction(), outwardNormal) < 0;
		normal = frontFace ? outwardNormal : (-1 * outwardNormal);
	}
};
*/

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
		auto scatterDirection = rec.normal + Vec3::randomUnitVector();
		// Catch degenerate scatter direction
		if (scatterDirection.nearZero())
		{
			scatterDirection = rec.normal;
		}
		scattered = ray(rec.p, scatterDirection);
		attenuation = albedo;
		return true;
	}
};

class Metal : public Material
{
public: 
	color3 albedo;
	double fuzz;
	Metal(const color3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(
		const ray& r_in, const hitRecord& rec, color3& attenuation, ray& scattered
	) const override {
		Vec3 reflected = reflect(unitVector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * Vec3::randomInUnitSphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
};

class Dielectric : public Material 
{
private:
	static double reflectance(double cosine, double refIdx)
	{
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - refIdx) / (1 + refIdx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
public:
	double ir; // Index of refraction
	Dielectric(double indexOfRefraction) : ir(indexOfRefraction) {}

	virtual bool scatter(
		const ray& r_in, const hitRecord& rec, color3& attenuation, ray& scattered
	) const override {
		attenuation = color3(1.0, 1.0, 1.0);
		double refractionRatio = rec.frontFace ? (1.0 / ir) : ir;

		Vec3 unitDirection = unitVector(r_in.direction());
		double cosTheta = fmin(dot((-1 * unitDirection), rec.normal), 1.0);
		double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

		bool cannotRefract = refractionRatio * sinTheta > 1.0;
		Vec3 direction;

		if (cannotRefract || reflectance(cosTheta, refractionRatio) > randomDouble())
		{
			direction = reflect(unitDirection, rec.normal);
		}
		else
		{
			direction = refract(unitDirection, rec.normal, refractionRatio);
		}
		scattered = ray(rec.p, direction);
		return true;
	}
};