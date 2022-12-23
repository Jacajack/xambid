#include "xambi/rgb_color.hpp"
#include <X11/Xutil.h>
#include <xambi/x11_image.hpp>
#include <utility>
#include <cassert>

using xambi::x11_image;

x11_image::x11_image(x11_context &xctx, XImage *img) :
	m_xctx(&xctx),
	m_img(img)
{
}

x11_image::x11_image(x11_image &&src) noexcept :
	m_img(std::exchange(src.m_img, nullptr)),
	m_xctx(src.m_xctx)
{
}

x11_image &x11_image::operator=(x11_image &&rhs) noexcept
{
	if (this == &rhs) return *this;
	m_img = std::exchange(rhs.m_img, nullptr);
	m_xctx = rhs.m_xctx;
	return *this;
}

x11_image::~x11_image()
{
	if (m_img)
		XDestroyImage(m_img);
}

xambi::rgb_color x11_image::get_pixel(int x, int y) const
{
	assert(m_img != nullptr);
	assert(m_xctx != nullptr);

#ifdef XAMBI_FAST_X11_IMAGE_GETPIXEL
	rgb_color color;
	uint32_t pixel = XGetPixel(m_img, x, y);
	color.r = (pixel & 0x000000ff) >> 0;
	color.g = (pixel & 0x0000ff00) >> 8;
	color.b = (pixel & 0x00ff0000) >> 16;
	return color;
#else
	XColor xcolor;
	xcolor.pixel = XGetPixel(m_img, x, y);
	xcolor = m_xctx->query_color(xcolor);

	rgb_color color;
	color.r = xcolor.red / 256;
	color.g = xcolor.green / 256;
	color.b = xcolor.blue / 256;
	return color;
#endif
}