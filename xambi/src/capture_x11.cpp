#include "xambi/rgb_color.hpp"
#include <xambi/capture_x11.hpp>
#include <xambi/x11_context.hpp>
#include <cassert>

using xambi::capture_x11;

capture_x11::capture_x11(x11_context &xctx) :
	m_xctx(&xctx),
	m_image(),
	m_width(m_xctx->get_display_width()),
	m_height(m_xctx->get_display_height())
{
}

void capture_x11::do_capture()
{
	m_image = x11_image{*m_xctx, XGetImage(
		m_xctx->get_display(),
		m_xctx->get_window(),
		0,
		0,
		m_xctx->get_window_width(),
		m_xctx->get_window_height(),
		AllPlanes,
		ZPixmap
	)};
}

void capture_x11::get_pixel_rect(rgb_color *dest, int x0, int y0, int w, int h)
{
	for (int y = y0; y < y0 + h; y++)
		for (int x = x0; x < x0 + w; x++)
			*dest++ = m_image.get_pixel(x, y);
}
