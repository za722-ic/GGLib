#include "GG/MoreMath/Vec3D.h"

GG::Vec3D GG::Vec3D::operator+(GG::Vec3D const& v) const
{
	GG::Vec3D out(x + v.x, y + v.y, z + v.z);
	return out;
}

GG::Vec3D& GG::Vec3D::operator+=(const GG::Vec3D& v)
{
	*this = *this + v;
	return *this;
}

GG::Vec3D GG::Vec3D::operator-(GG::Vec3D const& v) const
{
	GG::Vec3D out(x - v.x, y - v.y, z - v.z);
	return out;
}

GG::Vec3D& GG::Vec3D::operator-=(const GG::Vec3D& v)
{
	*this = *this - v;
	return *this;
}

GG::Vec3D GG::Vec3D::operator*(double scalar) const
{
	GG::Vec3D out(x * scalar, y * scalar, z * scalar);
	return out;
}

GG::Vec3D& GG::Vec3D::operator*=(double scalar)
{
	*this = *this * scalar;
	return *this;
}

GG::Vec3D GG::Vec3D::operator/(double scalar) const
{
	GG::Vec3D out(x / scalar, y / scalar, z / scalar);
	return out;
}

GG::Vec3D& GG::Vec3D::operator/=(double scalar)
{
	*this = *this / scalar;
	return *this;
}

double GG::Vec3D::dot(GG::Vec3D const& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

GG::Vec3D GG::Vec3D::cross(GG::Vec3D const& v) const
{
	return GG::Vec3D
	(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x
	);
}

double GG::Vec3D::magnitudeSquared() const
{
	return this->dot(*this);
}

double GG::Vec3D::magnitude() const
{
	return std::sqrt(magnitudeSquared());
}

void GG::Vec3D::normalize()
{
	*this /= magnitude();
}

GG::Vec3D GG::Vec3D::lerp(Vec3D const& end, float amount) const
{
	return *this + (end - *this) * amount;
}

double GG::Vec3D::getLinePlaneIntersection(GG::Vec3D pointOnPlane, GG::Vec3D normalToPlane, GG::Vec3D lineStart, GG::Vec3D lineEnd)
{
	/*
	Forumula from: https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection#Algebraic_form

	We only return the proportion along the line here.
	If you want the actual point on the line:

		Vec3D pointOnLine = lineStart + lineDir * d;
	*/

	GG::Vec3D lineDir = lineEnd - lineStart;

	if (lineDir.dot(normalToPlane) == 0) return std::numeric_limits<double>::infinity(); // line and plane are parallel so never intersect or the line is inside the plane. 0 or infinite solutions, so no single result! We'll say the point is infinitely along the line since idk what else is appropriate

	double d = (pointOnPlane - lineStart).dot(normalToPlane) / lineDir.dot(normalToPlane);

	return d;
}

GG::Vec3D GG::Vec3D::polarToCartesian(float latitude, float longitude, float radius)
{
	// https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
	// recall that we Y is up for us, not Z

	float outX = radius * sin(latitude) * cos(longitude);
	float outY = radius * cos(latitude);
	float outZ = radius * sin(latitude) * sin(longitude);
	GG::Vec3D out(outX, outY, outZ);

	return out;
}

std::string GG::Vec3D::toString()
{
	std::ostringstream outStr;
	outStr << std::fixed << std::setprecision(2) << x << ", " << y << ", " << z;
	return outStr.str();
}
