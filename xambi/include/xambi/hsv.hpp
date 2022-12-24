#pragma once
#include <algorithm>
#include <cmath>
#include <cassert>

namespace xambi {

inline void rgb_to_hsv(float r, float g, float b, float &h, float &s, float &v)
{
	assert(r >= 0 && r <= 1);
	assert(g >= 0 && g <= 1);
	assert(b >= 0 && b <= 1);

	const float xmax = std::max({r, g, b});
	const float xmin = std::min({r, g, b});
	const float c = xmax - xmin;
	const float l = (xmin + xmax) * 0.5f;
	v = xmax;

	if (c == 0)
	{
		h = 0;
	}
	else if (v == r)
	{
		h = 60.f / 360.f * (0.f + (g - b) / c);
	}
	else if (v == g)
	{
		h = 60.f / 360.f * (2.f + (b - r) / c);
	}
	else
	{
		h = 60.f / 360.f * (4.f + (r - g) / c);
	}

	if (h < 0)
		h = 1.f + h;

	h = std::fmod(h, 1.f);
	s = v == 0 ? 0 : c / v;

	assert(h >= 0 && h <= 1);
	assert(s >= 0 && s <= 1);
	assert(v >= 0 && v <= 1);
}

inline void hsv_to_rgb(float h, float s, float v, float &r, float &g, float &b)
{
	assert(h >= 0 && h <= 1);
	assert(s >= 0 && s <= 1);
	assert(v >= 0 && v <= 1);

	const float c = v * s;
	const float hp = h * 6.f;
	const float x = c * (1.f - std::abs(std::fmod(hp, 2.f) - 1));
	const float m = v - c;

	if (0 <= hp && hp < 1)
	{
		r = c + m;
		g = x + m;
		b = 0 + m;
	}
	else if (1 <= hp && hp < 2)
	{
		r = x + m;
		g = c + m;
		b = 0 + m;
	}
	else if (2 <= hp && hp < 3)
	{
		r = 0 + m;
		g = c + m;
		b = x + m;
	}
	else if (3 <= hp && hp < 4)
	{
		r = 0 + m;
		g = x + m;
		b = c + m;
	}
	else if (4 <= hp && hp < 5)
	{
		r = x + m;
		g = 0 + m;
		b = c + m;
	}
	else
	{
		r = c + m;
		g = 0 + m;
		b = x + m;
	}

	assert(r >= 0 && r <= 1);
	assert(g >= 0 && g <= 1);
	assert(b >= 0 && b <= 1);
}

}