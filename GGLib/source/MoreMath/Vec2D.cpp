#include "GG/MoreMath/Vec2D.h"

GG::Vec2D GG::Vec2D::operator+(GG::Vec2D const& v) const
{
	Vec2D out(x + v.x, y + v.y);
	return out;
}

GG::Vec2D& GG::Vec2D::operator+=(const GG::Vec2D& v)
{
	*this = *this + v;
	return *this;
}

GG::Vec2D GG::Vec2D::operator-(GG::Vec2D const& v) const {
	Vec2D out(x - v.x, y - v.y);
	return out;
}

GG::Vec2D& GG::Vec2D::operator-=(const GG::Vec2D& v)
{
	*this = *this - v;
	return *this;
}

GG::Vec2D GG::Vec2D::operator*(double scalar) const
{
	GG::Vec2D out(x * scalar, y * scalar);
	return out;
}

GG::Vec2D& GG::Vec2D::operator*=(double scalar)
{
	*this = *this * scalar;
	return *this;
}

GG::Vec2D GG::Vec2D::operator/(double scalar) const
{
	GG::Vec2D out(x / scalar, y / scalar);
	return out;
}

GG::Vec2D& GG::Vec2D::operator/=(double scalar)
{
	*this = *this / scalar;
	return *this;
}

double GG::Vec2D::dot(GG::Vec2D const& v) const
{
	return x * v.x + y * v.y;
}

double GG::Vec2D::magnitudeSquared() const
{
	return this->dot(*this);
}

double GG::Vec2D::magnitude() const
{
	return std::sqrt(magnitudeSquared());
}

void GG::Vec2D::normalize()
{
	*this /= magnitude();
}

GG::Vec2D GG::Vec2D::lerp(GG::Vec2D const& end, float amount) const
{
	return *this + (end - *this) * amount;
}

std::string GG::Vec2D::toString() const
{
	std::ostringstream outStr;
	outStr << std::fixed << std::setprecision(2) << x << ", " << y;
	return outStr.str();
}

SDL_Vertex GG::Vec2D::toSDLVertex(const SDL_Color& color) const
{
	SDL_Vertex sdlVertex =
	{
		SDL_FPoint(x, y),
		SDL_FColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f),
		SDL_FPoint {0}
	};
	return sdlVertex;
}
