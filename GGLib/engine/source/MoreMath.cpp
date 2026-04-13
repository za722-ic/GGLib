#include "MoreMath.h"

float MoreMath::lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

float MoreMath::map(float val, float domainLower, float domainUpper, float rangeLower, float rangeUpper)
{
	float frac = (val - domainLower) / (domainUpper - domainLower);
	return rangeLower + (rangeUpper - rangeLower) * frac;
}

float MoreMath::clamp(float val, float min, float max)
{
	return std::max(std::min(val, max), min); // there are implications for doing min after max if e.g, the user inputs a min that is greater than max --> in UI layouts, it is usually minWidth and minHeight that get the final say, not maxWidth/maxHeight
}

float MoreMath::mapAndClamp(float val, float domainLower, float domainUpper, float rangeLower, float rangeUpper)
{
	return clamp(map(val, domainLower, domainUpper, rangeLower, rangeUpper), rangeLower, rangeUpper);
}

float MoreMath::step(float x, float stepCenter)
{
	return x < stepCenter ? 0.0f : 1.0f;
}

float MoreMath::biLerp(float p00, float p01, float p10, float p11, float u, float v)
{
	float lerp0 = lerp(p00, p01, v); // left side
	float lerp1 = lerp(p10, p11, v); // right side

	return lerp(lerp0, lerp1, u);
}

float MoreMath::triLerp(float p000, float p001, float p010, float p011, float p100, float p101, float p110, float p111, float u, float v, float w)
{
	float lerp0 = biLerp(p000, p001, p010, p011, v, w); // bilinear interpolation of left face
	float lerp1 = biLerp(p100, p101, p110, p111, v, w); // bilinear interpolation of right face

	return lerp(lerp0, lerp1, u);
}