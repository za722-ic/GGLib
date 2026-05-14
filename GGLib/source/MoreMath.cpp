#include "MoreMath.h"

#include <algorithm>
#include <random>

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
	// why not use std::clamp? because it is undefined for min > max (documentation: https://en.cppreference.com/cpp/algorithm/clamp)
	// i want it to be well defined that if min > max, return min
	// this is useful in UI layouts, as it is usually minWidth and minHeight that get the final say, not maxWidth/maxHeight
	// hence why we std::max is the outermost function call below --> the final comparison is the one against the "min" argument (not to be confused with the std::min function)
	return std::max(std::min(val, max), min);
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

int MoreMath::random(int min, int max) {
	// this function is adapted from the example here: https://en.cppreference.com/cpp/numeric/random/uniform_int_distribution


	// static --> don't want to recreate over-and-over
	// see example here: https://en.cppreference.com/cpp/numeric/random/random_device
	// "For practical use random_device is generally only used to a PRNG such as mt19937"
	static std::random_device rd;
	static std::mt19937 gen(rd());

	// create uniform distribution
	std::uniform_int_distribution<int> distrib(min, max);

	return distrib(gen);
}
