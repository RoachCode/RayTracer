#pragma once
#include "Weekend.h"

class Camera
{
private:
	point3 origin;
	point3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
	
public:
	Camera()
	{
		auto aspectRatio = 16.0 / 9.0;
		auto viewportH = 2.0;
		auto viewportW = aspectRatio * viewportH;
		auto focalLength = 1.0;

		origin = point3(0, 0, 0);
		horizontal = Vec3(viewportW, 0.0, 0.0);
		vertical = Vec3(0.0, viewportH, 0.0);
		lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focalLength);
	}
	ray getRay(double u, double v) const
	{
		return ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
	}
};
