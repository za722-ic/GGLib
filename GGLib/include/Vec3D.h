#pragma once

#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

struct Vec3D
{
	double x, y, z;

	Vec3D(double x, double y, double z) : x(x), y(y), z(z)
	{}

	Vec3D() : Vec3D(0.0f, 0.0f, 0.0f)
	{}

	Vec3D(double n) : Vec3D(n, n, n)
	{}

	Vec3D operator+(Vec3D const& v) const;

	Vec3D& operator+=(const Vec3D& v);

	Vec3D operator-(Vec3D const& v) const;
	
	Vec3D& operator-=(const Vec3D& v);

	Vec3D operator*(double scalar) const;

	Vec3D& operator*=(double scalar);

	Vec3D operator/(double scalar) const;

	Vec3D& operator/=(double scalar);
	
	double dot(Vec3D const& v) const;

	Vec3D cross(Vec3D const& v) const;

	double magnitudeSquared() const;

	double magnitude() const;

	void normalize();

	Vec3D lerp(Vec3D const& end, float amount) const;

	// TODO: What if there is no single point oof intersection. There could be infinitely many if the line is parallel to and inside the plane, or 0 if parallel and outside the plane
	static double getLinePlaneIntersection(Vec3D pointOnPlane, Vec3D normalToPlane, Vec3D lineStart, Vec3D lineEnd);
	
	static Vec3D polarToCartesian(float latitude, float longitude, float radius);

	std::string toString();
};