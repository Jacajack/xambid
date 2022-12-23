#include <xambi/screen_sampler.hpp>

using xambi::screen_sampler;

screen_sampler::screen_sampler(capture_backend &capture, const screen_rect &rect) :
	m_capture(&capture),
	m_rect(rect)
{
}