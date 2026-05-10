#pragma once

#include <SDL3/SDL.h>

#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

struct Vec2D
{
	double x, y;

	Vec2D(double x, double y) : x(x), y(y)
	{}

	Vec2D() : Vec2D(0.0f, 0.0f)
	{}

	Vec2D(double n) : Vec2D(n, n)
	{}

	Vec2D operator+(Vec2D const& v) const;

	Vec2D& operator+=(const Vec2D& v);

	Vec2D operator-(Vec2D const& v) const;

	Vec2D& operator-=(const Vec2D& v);

	Vec2D operator*(double scalar) const;

	Vec2D& operator*=(double scalar);

	Vec2D operator/(double scalar) const;

	Vec2D& operator/=(double scalar);

	double dot(Vec2D const& v) const;

	double magnitudeSquared() const;

	double magnitude() const;

	void normalize();

	// linearly interpolate from this vector to end by amount
	Vec2D lerp(Vec2D const& end, float amount) const;

	std::string toString() const;

	SDL_Vertex toSDLVertex(const SDL_Color& color) const;
};
