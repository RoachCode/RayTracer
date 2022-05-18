#pragma once
#include <cmath>
#include <iostream>

class Vec3
{
public:

#pragma region initializeDeclare

	// Declare an array of three double values
	double e[3];

	// The following looks a little awkward - this is the only way to initialize const values.
	// Initializes array with zero values
	Vec3() : e{ 0, 0, 0 } {}
	// Assigns values to array
	Vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

#pragma endregion
#pragma region operatorOverloading

	// Adds the values of a Vec3 to the existing Vec3
	Vec3 &operator += (const Vec3 &v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	// Multiplies each value in the Vec3 by const double 't'
	Vec3 &operator *= (const double t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
	}

	// Divides each value in the Vec3 by const double 't'
	Vec3 &operator /= (const double t)
	{
		return *this *= 1 / t;
	}

	// Outputs Vec3 values to the standard output stream delimited by ' ' spaces
	inline friend std::ostream &operator << (std::ostream& out, const Vec3& v) 
	{
		return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
	}

	// Adds two Vec3 together. 'u' stands for utility
	inline friend Vec3 operator + (const Vec3& u, const Vec3& v) 
	{
		return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
	}

	// Takes two Vec3. Subtracts the 2nd Vec3 from the first.
	inline friend Vec3 operator - (const Vec3& u, const Vec3& v) 
	{
		return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
	}

	// Multiplies two Vec3 together if both arguments are Vec3
	inline friend Vec3 operator * (const Vec3& u, const Vec3& v) 
	{
		return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
	}

	// Multiplies a double and Vec3, returns Vec3
	inline friend Vec3 operator * (double t, const Vec3& v) 
	{
		return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
	}

	// Multiplies a Vec3 and double, returns double. I don't know how this works TODO
	inline friend Vec3 operator * (const Vec3& v, double t) 
	{
		return t * v;
	}

	// Divides a Vec3 by a double, returns double. I don't know how this works TODO
	inline friend Vec3 operator / (Vec3 v, double t) 
	{
		return (1 / t) * v;
	}

#pragma endregion
#pragma region utilityFunctions

	// Position returns
	const double x() { return e[0]; }
	const double y() { return e[1]; }
	const double z() { return e[2]; }

	const double lengthSquared()
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	const double length()
	{
		return std::sqrt(lengthSquared());
	}

	// Multiplies each corresponding value of two Vec3 together, returns a Vec3
	inline friend double dot(const Vec3& u, const Vec3& v) 
	{
		return u.e[0] * v.e[0]
			+ u.e[1] * v.e[1]
			+ u.e[2] * v.e[2];
	}

	// I assume this takes a line and returns a perpendicular line
	inline friend Vec3 cross(const Vec3& u, const Vec3& v) 
	{
		return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
			u.e[2] * v.e[0] - u.e[0] * v.e[2],
			u.e[0] * v.e[1] - u.e[1] * v.e[0]);
	}

	// Returns Vec3 / Vec3.length();
	inline friend Vec3 unitVector(Vec3 v)
	{
		return v / v.length();
	}

	inline static Vec3 random()
	{
		return Vec3(randomDouble(), randomDouble(), randomDouble());
	}

	inline static Vec3 random(double min, double max)
	{
		return Vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
	}

	static Vec3 randomInUnitSphere()
	{
		while (true)
		{
			auto p = Vec3::random(-1, 1);
			if (p.lengthSquared() >= 1) continue;
			return p;
		}
	}

	static Vec3 randomUnitVector()
	{
		return unitVector(randomInUnitSphere());
	}

#pragma endregion

};

using point3 = Vec3;   // 3D point
using color3 = Vec3;    // RGB color
