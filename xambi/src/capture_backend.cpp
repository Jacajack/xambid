#include "xambi/rgb_color.hpp"
#include <xambi/capture_backend.hpp>

using xambi::capture_backend;

xambi::rgb_color capture_backend::get_pixel(int x, int y)
{
	rgb_color c;
	get_pixel_rect(&c, x, y, 1, 1);
	return c;
}