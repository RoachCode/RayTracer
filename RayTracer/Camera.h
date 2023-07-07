#pragma once
#include "Weekend.h"

class Camera
{
private:
	point3 origin;
	point3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u, v, w;
	double lensRadius;

public:
	
	// point3 lookFrom, point3 lookAt, Vec3 vUp, double vfov, Vec3 vertical
	Camera(
		point3 lookFrom,
		point3 lookAt,
		Vec3   vUp,
		double vfov, // vertical field-of-view in degrees
		double aspectRatio,
		double aperture,
		double focusDist
	) {
		auto theta = degreesToRadians(vfov);
		auto h = tan(theta / 2);
		auto viewportHeight = 2.0 * h;
		auto viewportWidth = aspectRatio * viewportHeight;

		w = unitVector(lookFrom - lookAt);
		u = unitVector(cross(vUp, w));
		v = cross(w, u);

		origin = lookFrom;
		horizontal = focusDist * viewportWidth * u;
		vertical = focusDist * viewportHeight * v;
		lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - focusDist * w;

		lensRadius = aperture / 2;
	}
	void redefine(
		point3 lookFrom,
		point3 lookAt,
		Vec3   vUp,
		double vfov, // vertical field-of-view in degrees
		double aspectRatio,
		double aperture,
		double focusDist
	) {
		auto theta = degreesToRadians(vfov);
		auto h = tan(theta / 2);
		auto viewportHeight = 2.0 * h;
		auto viewportWidth = aspectRatio * viewportHeight;

		w = unitVector(lookFrom - lookAt);
		u = unitVector(cross(vUp, w));
		v = cross(w, u);

		origin = lookFrom;
		horizontal = focusDist * viewportWidth * u;
		vertical = focusDist * viewportHeight * v;
		lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - focusDist * w;

		lensRadius = aperture / 2;
	}
	ray getRay(double s, double t) const
	{
		Vec3 rd = Vec3::randomInUnitDisk() * lensRadius;
		Vec3 offset = u * rd.x() + v * rd.y();

		return ray(
			origin + offset,
			lowerLeftCorner + s * horizontal + t * vertical - origin - offset
		);
	}
};
