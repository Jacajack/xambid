#pragma once
#include "rgb_color.hpp"

namespace xambi {

class color_aggregate
{
public:
	color_aggregate() = default;
	virtual ~color_aggregate() = default;

	virtual rgb_color process(const rgb_color *data, std::size_t size) = 0;
	virtual float get_confidence() const {return 1.f;}
};

}