#include "xambi/rgb_color.hpp"
#include <xambi/naive_screen_sampler.hpp>
#include <xambi/capture_backend.hpp>

using xambi::naive_screen_sampler;

std::vector<xambi::rgb_color> naive_screen_sampler::sample() const
{
	std::vector<rgb_color> pixels(m_rect.w * m_rect.h);
	m_capture->get_pixel_rect(pixels.data(), m_rect.x, m_rect.y, m_rect.w, m_rect.h);
	return pixels;
}