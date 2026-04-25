#pragma once

#include <algorithm>

namespace MoreMath
{
	float lerp(float a, float b, float t);
	
	float map(float val, float domanLower, float domainUpper, float rangeLower, float rangeUpper);

	float clamp(float val, float min, float max);

	float mapAndClamp(float val, float domainLower, float domainUpper, float rangeLower, float rangeUpper);

    // step function, with the "step" at stepCenter
    float step(float x, float stepCenter);

    // bilinear interpolation (mixing the 4 values at the vertices of a square)
    float biLerp(float p00, float p01, float p10, float p11, float u, float v);

    // trilinear interpolation (mixing the 8 values at the vertices of a cube)
    float triLerp(float p000, float p001, float p010, float p011, float p100, float p101, float p110, float p111, float u, float v, float w);
}