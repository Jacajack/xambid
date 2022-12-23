#include <xambi/x11_context.hpp>
#include <cassert>

x11_context::x11_context()
{
	m_display = XOpenDisplay(NULL);
	assert(m_display != NULL);
	
	m_screen = DefaultScreen(m_display);
	m_window = RootWindow(m_display, m_screen);
}

x11_context::~x11_context()
{
	XCloseDisplay(m_display);
}

XColor x11_context::query_color(XColor color) const
{
	XQueryColor(m_display, DefaultColormap(m_display, m_screen), &color);
	return color;
}

XWindowAttributes x11_context::get_window_attribs() const
{
	XWindowAttributes attribs;
	XGetWindowAttributes(m_display, m_window, &attribs);
	return attribs;
}

