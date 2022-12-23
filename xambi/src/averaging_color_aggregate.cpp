#include "xambi/color_aggregate.hpp"
#include <xambi/averaging_color_aggregate.hpp>

using xambi::averaging_color_aggregate;

xambi::rgb_color averaging_color_aggregate::process(const rgb_color *data, std::size_t size)
{
	uint64_t r = 0;
	uint64_t g = 0;
	uint64_t b = 0;

	for (std::size_t i = 0; i < size; i++)
	{
		r += data[i].r;
		g += data[i].g;
		b += data[i].b;
	}

	rgb_color color;
	color.r = r / size;
	color.g = g / size;
	color.b = b / size;
	return color;
}