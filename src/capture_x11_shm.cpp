#include "capture_x11_shm.hpp"
#include <cassert>
#include <stdexcept>
#include <sys/shm.h>
#include <X11/Xutil.h>
#define FULLPTR ((void*)-1)
 
capture_x11_shm::capture_x11_shm(x11_context &xctx) :
	m_xctx(&xctx),
	m_width(m_xctx->get_window_width()),
	m_height(m_xctx->get_window_height())
{
	if (!XShmQueryExtension(m_xctx->get_display()))
		throw std::runtime_error{"X11 Shm extension not supported"};
	
	m_image = XShmCreateImage(
		m_xctx->get_display(),
		m_xctx->get_default_visual(),
		m_xctx->get_default_depth(),
		ZPixmap,
		nullptr,
		&m_shminfo,
		m_width,
		m_height
	);
	
	assert(m_image != nullptr);
	
	m_shminfo.shmid = shmget(
		IPC_PRIVATE,
		m_image->bytes_per_line * m_image->height,
		IPC_CREAT | 0600);
	assert(m_shminfo.shmid != -1);
	
	m_shminfo.shmaddr = m_image->data = reinterpret_cast<char*>(shmat(m_shminfo.shmid, 0, 0));
	m_shminfo.readOnly = false;
	
	XShmAttach(m_xctx->get_display(), &m_shminfo);
}

capture_x11_shm::~capture_x11_shm()
{
	XShmDetach(m_xctx->get_display(), &m_shminfo);
	XDestroyImage(m_image);
	shmdt(m_shminfo.shmaddr);
	shmctl(m_shminfo.shmid, IPC_RMID, 0);
}
 
void capture_x11_shm::do_capture()
{
	XShmGetImage(
		m_xctx->get_display(),
		m_xctx->get_window(),
		m_image,
		0,
		0,
		AllPlanes);
}

void capture_x11_shm::get_pixel(int x, int y, float &r, float &g, float &b)
{
	// TODO move this to image type
	assert(m_image != nullptr);
	
	XColor color;
	color.pixel = XGetPixel(m_image, x, y);
	color = m_xctx->query_color(color);
	r = color.red / 65535.f;
	g = color.green / 65535.f;
	b = color.blue / 65535.f;
}
 
