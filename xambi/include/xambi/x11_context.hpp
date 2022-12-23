#pragma once
#include "utils.hpp"
#include <X11/Xlib.h>

class x11_context : public no_copy, public no_move
{
public:
	x11_context();
	~x11_context();
	
	auto get_display() const {return m_display;}
	auto get_window() const {return m_window;}
	auto get_screen() const {return m_screen;}
	auto get_default_visual() const {return DefaultVisual(get_display(), get_screen());}
	auto get_default_depth() const {return DefaultDepth(get_display(), get_screen());}
	
	XColor query_color(XColor c) const;
	XWindowAttributes get_window_attribs() const;
	int get_window_width() const {return get_window_attribs().width;}
	int get_window_height() const {return get_window_attribs().height;}
	int get_display_width() const {return DisplayWidth(m_display, m_screen);}
	int get_display_height() const {return DisplayHeight(m_display, m_screen);}

private:
	Display *m_display;
	Window m_window;
	int m_screen;
};
