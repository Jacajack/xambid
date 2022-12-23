#pragma once
#include "screen_zone.hpp"

class capture_api;

class screen_zone_sampler
{
public:
	screen_zone_sampler(const screen_zone &zone, const capture_api &api);
	
	virtual void sample(float dt = 0);
	
protected:
	screen_zone m_zone;
	capture_api *m_capture;
};


class random_screen_zone_sampler : public screen_zone_sampler
{
	
};

class naive_screen_sampler : public screen_zone_sampler
{

};

