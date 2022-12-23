#pragma once
#include "screen_sampler.hpp"
#include "capture_backend.hpp"
#include <random>
#include <cassert>

namespace xambi {

template <typename RNG>
class random_screen_sampler : public screen_sampler
{
public:
	explicit random_screen_sampler(capture_backend &capture, const screen_rect &rect, RNG &rng, float p);
	std::vector<rgb_color> sample() const override;	

protected:
	RNG *m_rng;
	float m_probability;
};

template <typename RNG>
random_screen_sampler<RNG>::random_screen_sampler(capture_backend &capture, const screen_rect &rect, RNG &rng, float p) :
	screen_sampler(capture, rect),
	m_rng(&rng),
	m_probability(p)
{
}

template <typename RNG>
std::vector<xambi::rgb_color> random_screen_sampler<RNG>::sample() const
{
	assert(m_probability < 1.f);

	int samples = m_rect.w * m_rect.h * m_probability;
	std::vector<rgb_color> pixels;
	pixels.reserve(samples);

	std::uniform_int_distribution<int> xdist(m_rect.x, m_rect.x + m_rect.w - 1);
	std::uniform_int_distribution<int> ydist(m_rect.y, m_rect.y + m_rect.h - 1);

	for (int i = 0; i < samples; i++)
	{
		int x = xdist(*m_rng);
		int y = ydist(*m_rng);
		pixels.push_back(m_capture->get_pixel(x, y));
	}

	return pixels;
}

}