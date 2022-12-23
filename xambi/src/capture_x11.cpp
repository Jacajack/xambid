#include <xambi/capture_x11.hpp>
#include <xambi/x11_context.hpp>
#include <cassert>

capture_x11::capture_x11(x11_context &xctx) :
	m_xctx(&xctx),
	m_image(nullptr),
	m_width(m_xctx->get_display_width()),
	m_height(m_xctx->get_display_height())
{
}

capture_x11::~capture_x11()
{
	free_image();
}

void capture_x11::do_capture()
{
	free_image();
	m_image = XGetImage(
		m_xctx->get_display(),
		m_xctx->get_window(),
		0,
		0,
		m_xctx->get_window_width(),
		m_xctx->get_window_height(),
		AllPlanes,
		ZPixmap);
}

void capture_x11::get_pixel(int x, int y, float &r, float &g, float &b)
{
	assert(m_image != nullptr);
	
	XColor color;
	color.pixel = XGetPixel(m_image, x, y);
	color = m_xctx->query_color(color);
	r = color.red / 65535.f;
	g = color.green / 65535.f;
	b = color.blue / 65535.f;
}

void capture_x11::free_image()
{
	if (!m_image) return;
	XDestroyImage(m_image);
}
