#pragma once
#include "color_aggregate.hpp"

namespace xambi {

class averaging_color_aggregate : public color_aggregate
{
public:
	rgb_color process(const rgb_color *data, std::size_t size) override;
};

}