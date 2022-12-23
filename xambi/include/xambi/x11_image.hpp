#pragma once
#include "rgb_color.hpp"
#include "xambi/x11_context.hpp"
#include <X11/Xlib.h>

class x11_context;

class x11_image
{
public:
	x11_image() = default;
	x11_image(x11_context &xctx, XImage *img);
	x11_image(const x11_image &) = delete;
	x11_image(x11_image &&src) noexcept;
	x11_image &operator=(const x11_image &) = delete;
	x11_image &operator=(x11_image &&rhs) noexcept;
	~x11_image();

	auto ptr() {return m_img;}
	auto ptr() const {return m_img;}
	auto &operator*() {return *m_img;}
	auto &operator*() const {return *m_img;}
	auto operator->() {return m_img;}
	auto operator->() const {return m_img;}

	rgb_color get_pixel(int x, int y) const;

private:
	x11_context *m_xctx = nullptr;
	XImage *m_img = nullptr;
};