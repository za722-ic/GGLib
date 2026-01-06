#pragma once

#include <algorithm>

namespace MoreMath
{
	float lerp(float a, float b, float t);
	
	float map(float val, float domanLower, float domainUpper, float rangeLower, float rangeUpper);

	float clamp(float val, float min, float max);
}