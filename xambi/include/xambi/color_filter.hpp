#pragma once
#include "rgb_color.hpp"

class color_filter
{
public:
	color_filter() = default;
	virtual ~color_filter() = default;

	virtual rgb_color feed(const rgb_color &x, float dt) = 0;
	virtual rgb_color out() const = 0;
};

class passthrough_color_filter : public color_filter
{
public:
	rgb_color feed(const rgb_color &x, float dt) override {return m_color = x;}
	rgb_color out() const override {return m_color;}

private:
	rgb_color m_color;
};