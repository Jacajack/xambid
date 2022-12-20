#pragma once

class capture_api
{
public:
	virtual ~capture_api() = default;
	
	virtual int get_width() const = 0;
	virtual int get_height() const = 0;
	virtual void do_capture() = 0;
	virtual void get_pixel(int x, int y, float &r, float &g, float &b) = 0;
};
