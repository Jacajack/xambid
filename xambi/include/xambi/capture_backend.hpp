#pragma once
#include "rgb_color.hpp"

namespace xambi {

class capture_backend
{
public:
	virtual ~capture_backend() = default;
	
	virtual int get_width() const = 0;
	virtual int get_height() const = 0;
	virtual void do_capture() = 0;

	virtual rgb_color get_pixel(int x, int y);
	virtual void get_pixel_rect(rgb_color *dest, int x0, int y0, int w, int h) = 0;
};

}