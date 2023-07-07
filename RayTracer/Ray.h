#pragma once
#include "Vec3.h"

class ray
{
public:
    point3 orig;
    Vec3 dir;
    ray() {}
    ray(const point3& origin, const Vec3& direction)
        : orig(origin), dir(direction)
    {}

    point3 origin() const { return orig; }
    Vec3 direction() const { return dir; }

    // Linear interpolation 
    point3 at(double t) const
    {
        return orig + t * dir;
    }


};
