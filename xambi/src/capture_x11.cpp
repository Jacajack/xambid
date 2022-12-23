#include <xambi/capture_x11.hpp>
#include <xambi/x11_context.hpp>
#include <cassert>

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

void capture_x11::get_pixel(int x, int y, float &r, float &g, float &b)
{
	auto pixel = m_image.get_pixel(x, y);
	r = pixel.r / 255.f;
	g = pixel.g / 255.f;
	b = pixel.b / 255.f;
}
