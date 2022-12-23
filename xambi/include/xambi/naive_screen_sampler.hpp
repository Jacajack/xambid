#pragma once
#include "screen_sampler.hpp"

namespace xambi {

class naive_screen_sampler : public screen_sampler
{
public:
	using screen_sampler::screen_sampler;
	std::vector<rgb_color> sample() const override;
};

}