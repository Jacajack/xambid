#include "xambi/rgb_color.hpp"
#include <xambi/lowpass_color_filter.hpp>
#include <cmath>

using xambi::lowpass_color_filter;

lowpass_color_filter::lowpass_color_filter(float tau) :
	m_tau(tau)
{
}

xambi::rgb_color lowpass_color_filter::feed(const rgb_color &x, float dt)
{
	m_r = filter(m_r, x.r / 255.f, dt);
	m_g = filter(m_g, x.g / 255.f, dt);
	m_b = filter(m_b, x.b / 255.f, dt);
	return out();
}

xambi::rgb_color lowpass_color_filter::out() const
{
	rgb_color c;
	c.r = m_r * 255.f;
	c.g = m_g * 255.f;
	c.b = m_b * 255.f;
	return c;
}

float lowpass_color_filter::filter(float state, float x, float dt) const
{
	const float u = std::exp(-dt / m_tau);
	return u * state + (1.f - u) * x;
}