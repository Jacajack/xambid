#pragma once
#include <vector>
#include "screen_rect.hpp"
#include "rgb_color.hpp"

namespace xambi {

class capture_backend;

class screen_sampler
{
public:
	screen_sampler(capture_backend &capture, const screen_rect &rect);
	virtual std::vector<rgb_color> sample() const = 0;
	
protected:
	capture_backend *m_capture;
	screen_rect m_rect;
};

}