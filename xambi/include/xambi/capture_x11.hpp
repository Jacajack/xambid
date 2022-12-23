#pragma once
#include <X11/Xutil.h>
#include "capture_backend.hpp"
#include "utils.hpp"
#include "x11_context.hpp"
#include "x11_image.hpp"

namespace xambi {

class capture_x11 : public capture_backend, public no_copy, public no_move
{
public:
	explicit capture_x11(x11_context &xctx);
	
	int get_width() const override {return m_width;}
	int get_height() const override {return m_height;}
	void do_capture() override;
	void get_pixel_rect(rgb_color *dest, int x0, int y0, int w, int h) override;

private:
	x11_context *m_xctx;
	x11_image m_image;
	int m_width;
	int m_height;
};

}