#include "Vec2D.h"

Vec2D Vec2D::operator+(Vec2D const& v) const
{
	Vec2D out(x + v.x, y + v.y);
	return out;
}

Vec2D& Vec2D::operator+=(const Vec2D& v)
{
	*this = *this + v;
	return *this;
}

Vec2D Vec2D::operator-(Vec2D const& v) const{
	Vec2D out(x - v.x, y - v.y);
	return out;
}

Vec2D& Vec2D::operator-=(const Vec2D& v)
{
	*this = *this - v;
	return *this;
}

Vec2D Vec2D::operator*(double scalar) const
{
	Vec2D out(x * scalar, y * scalar);
	return out;
}

Vec2D& Vec2D::operator*=(double scalar)
{
	*this = *this * scalar;
	return *this;
}

Vec2D Vec2D::operator/(double scalar) const
{
	Vec2D out(x / scalar, y / scalar);
	return out;
}

Vec2D& Vec2D::operator/=(double scalar)
{
	*this = *this / scalar;
	return *this;
}

double Vec2D::dot(Vec2D const& v) const
{
	return x * v.x + y * v.y;
}

double Vec2D::magnitudeSquared() const
{
	return this->dot(*this);
}

double Vec2D::magnitude() const
{
	return std::sqrt(magnitudeSquared());
}

void Vec2D::normalize()
{
	*this /= magnitude();
}

Vec2D Vec2D::lerp(Vec2D const& end, float amount) const
{
	return *this + (end - *this) * amount;
}

std::string Vec2D::toString() const
{
	std::ostringstream outStr;
	outStr << std::fixed << std::setprecision(2) << x << ", " << y;
	return outStr.str();
}

SDL_Vertex Vec2D::toSDLVertex(const SDL_Color& color) const
{
	SDL_Vertex sdlVertex =
	{
		SDL_FPoint(x, y),
		SDL_FColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f),
		SDL_FPoint {0}
	};
	return sdlVertex;
}
