#pragma once
#include "rgb_color.hpp"

class color_aggregate
{
public:
	color_aggregate() = default;
	virtual ~color_aggregate() = default;

	virtual rgb_color process(const rgb_color *data, std::size_t size) = 0;
};

class averaging_color_aggregate : public color_aggregate
{
};

class color_mode_aggregate : public color_aggregate
{
};