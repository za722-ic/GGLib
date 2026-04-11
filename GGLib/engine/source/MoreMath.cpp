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