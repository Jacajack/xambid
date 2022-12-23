#pragma once
#include "color_filter.hpp"

namespace xambi {

class lowpass_color_filter : public color_filter
{
public:
	explicit lowpass_color_filter(float tau);
	rgb_color feed(const rgb_color &x, float dt) override;
	rgb_color out() const override;

private:
	float filter(float state, float x, float dt) const;

	float m_tau;
	float m_r = 0;
	float m_g = 0;
	float m_b = 0;
};

}