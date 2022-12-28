#pragma once
#include "screen_sampler.hpp"
#include "capture_backend.hpp"
#include <algorithm>
#include <random>
#include <cassert>

namespace xambi {

template <typename RNG>
class stratified_screen_sampler : public screen_sampler
{
public:
	explicit stratified_screen_sampler(
		capture_backend &capture,
		const screen_rect &rect,
		RNG &rng,
		int stratum_width,
		int stratum_height,
		int samples_per_stratum);
	std::vector<rgb_color> sample() const override;	

protected:
	RNG *m_rng;
	std::vector<screen_rect> m_strata;
	int m_samples_per_stratum;
};

template <typename RNG>
stratified_screen_sampler<RNG>::stratified_screen_sampler(
	capture_backend &capture,
	const screen_rect &rect,
	RNG &rng,
	int stratum_width,
	int stratum_height,
	int samples_per_stratum) :
	screen_sampler(capture, rect),
	m_rng(&rng),
	m_samples_per_stratum(samples_per_stratum)
{
	for (int y = rect.y; y < rect.y + rect.h; y += stratum_height)
		for (int x = rect.x; x < rect.x + rect.w; x += stratum_width)
		{
			int w = std::min(stratum_width, rect.x + rect.w - x);
			int h = std::min(stratum_height, rect.y + rect.h - y);
			m_strata.push_back(screen_rect{x, y, w, h});
		}
	
	std::reverse(m_strata.begin(), m_strata.end());
}

template <typename RNG>
std::vector<xambi::rgb_color> stratified_screen_sampler<RNG>::sample() const
{
	int samples = m_strata.size() * m_samples_per_stratum;
	std::vector<rgb_color> pixels;
	pixels.reserve(samples);

	for (const auto &stratum : m_strata)
	{
		std::uniform_int_distribution<int> xdist(stratum.x, stratum.x + stratum.w - 1);
		std::uniform_int_distribution<int> ydist(stratum.y, stratum.y + stratum.h - 1);

		for (int i = 0; i < m_samples_per_stratum; i++)
		{
			int x = xdist(*m_rng);
			int y = ydist(*m_rng);
			pixels.push_back(m_capture->get_pixel(x, y));
		}
	}

	return pixels;
}

}