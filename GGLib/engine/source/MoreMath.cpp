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
	return std::max(std::min(val, max), min);
}

float MoreMath::mapAndClamp(float val, float domainLower, float domainUpper, float rangeLower, float rangeUpper)
{
	return clamp(map(val, domainLower, domainUpper, rangeLower, rangeUpper), rangeLower, rangeUpper);
}