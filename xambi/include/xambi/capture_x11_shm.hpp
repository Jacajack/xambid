#pragma once
#include "capture_api.hpp"
#include "utils.hpp"
#include "x11_context.hpp"
#include <X11/extensions/XShm.h>

class capture_x11_shm : public capture_api, public no_copy, public no_move
{
public:
	explicit capture_x11_shm(x11_context &xctx);
	~capture_x11_shm() override;
	
	int get_width() const override {return m_width;}
	int get_height() const override {return m_height;}
	void do_capture() override;
	void get_pixel(int x, int y, float &r, float &g, float &b) override;

private:
	x11_context *m_xctx;
	XImage *m_image;
	XShmSegmentInfo m_shminfo;
	int m_width;
	int m_height;
};
