#pragma once
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include "capture_backend.hpp"
#include "utils.hpp"
#include "x11_context.hpp"
#include "x11_image.hpp"

namespace xambi {

class capture_x11_shm : public capture_backend, public no_copy, public no_move
{
public:
	explicit capture_x11_shm(x11_context &xctx);
	~capture_x11_shm() override;
	
	int get_width() const override {return m_width;}
	int get_height() const override {return m_height;}
	void do_capture() override;
	void get_pixel_rect(rgb_color *dest, int x0, int y0, int w, int h) override;

private:
	x11_context *m_xctx;
	x11_image m_image;
	XShmSegmentInfo m_shminfo;
	int m_width;
	int m_height;
};

}