#include <xambi/capture_x11_nvfbc.hpp>
#include <xambi/nv/NvFBC.h>
#include <cassert>
#include <dlfcn.h>

#define APP_VERSION 3
#define LIB_NVFBC_NAME "libnvidia-fbc.so.1"

capture_x11_nvfbc::capture_x11_nvfbc(x11_context &xctx) :
	m_xctx(&xctx),
	m_lib_nvfbc(nullptr),
	m_buffer(nullptr),
	m_width(m_xctx->get_display_width()),
	m_height(m_xctx->get_display_height())
{
	m_lib_nvfbc = dlopen(LIB_NVFBC_NAME, RTLD_NOW);
	assert(m_lib_nvfbc != nullptr);
	
	auto NvFBCCreateInstance_ptr = (PNVFBCCREATEINSTANCE) dlsym(m_lib_nvfbc, "NvFBCCreateInstance");
	assert(NvFBCCreateInstance_ptr != nullptr);
	
	NVFBCSTATUS status;
	m_nvfbc = {};
	m_nvfbc.dwVersion = NVFBC_VERSION;
	status = NvFBCCreateInstance_ptr(&m_nvfbc);
	assert(status == NVFBC_SUCCESS);
	
	NVFBC_CREATE_HANDLE_PARAMS fbc_create_handle_params{};
	fbc_create_handle_params.dwVersion = NVFBC_CREATE_HANDLE_PARAMS_VER;
	status = m_nvfbc.nvFBCCreateHandle(&m_fbc_handle, &fbc_create_handle_params);
	assert(status == NVFBC_SUCCESS);
	
	NVFBC_GET_STATUS_PARAMS fbc_get_status_params{};
	fbc_get_status_params.dwVersion = NVFBC_GET_STATUS_PARAMS_VER;
	status = m_nvfbc.nvFBCGetStatus(m_fbc_handle, &fbc_get_status_params);
	assert(status == NVFBC_SUCCESS);
	assert(fbc_get_status_params.bCanCreateNow == NVFBC_TRUE);
	
	NVFBC_BOX capture_box{};
	capture_box.x = 0;
	capture_box.y = 0;
	capture_box.w = get_width();
	capture_box.h = get_height();
	
	NVFBC_SIZE frame_size{};
	frame_size.w = capture_box.w;
	frame_size.h = capture_box.h;
	
	NVFBC_CREATE_CAPTURE_SESSION_PARAMS fbc_create_capture_params{};
	fbc_create_capture_params.dwVersion = NVFBC_CREATE_CAPTURE_SESSION_PARAMS_VER;
	fbc_create_capture_params.eCaptureType = NVFBC_CAPTURE_TO_SYS;
	fbc_create_capture_params.bWithCursor = NVFBC_FALSE;
	fbc_create_capture_params.captureBox = capture_box;
	fbc_create_capture_params.frameSize = frame_size;
	fbc_create_capture_params.eTrackingType = NVFBC_TRACKING_SCREEN;
	status = m_nvfbc.nvFBCCreateCaptureSession(m_fbc_handle, &fbc_create_capture_params);
	assert(status == NVFBC_SUCCESS);
	
	NVFBC_TOSYS_SETUP_PARAMS setup_params{};
	setup_params.dwVersion = NVFBC_TOSYS_SETUP_PARAMS_VER;
	setup_params.eBufferFormat = NVFBC_BUFFER_FORMAT_RGB;
	setup_params.ppBuffer = reinterpret_cast<void**>(&m_buffer);
	setup_params.bWithDiffMap = NVFBC_FALSE;
	status = m_nvfbc.nvFBCToSysSetUp(m_fbc_handle, &setup_params);
	assert(status == NVFBC_SUCCESS);
}

capture_x11_nvfbc::~capture_x11_nvfbc()
{
	NVFBCSTATUS status;
	NVFBC_DESTROY_CAPTURE_SESSION_PARAMS destroy_capture_params{};
	destroy_capture_params.dwVersion = NVFBC_DESTROY_CAPTURE_SESSION_PARAMS_VER;
	status = m_nvfbc.nvFBCDestroyCaptureSession(m_fbc_handle, &destroy_capture_params);
	assert(status == NVFBC_SUCCESS);
	
	NVFBC_DESTROY_HANDLE_PARAMS destroy_handle_params{};
	destroy_handle_params.dwVersion = NVFBC_DESTROY_HANDLE_PARAMS_VER;
	status = m_nvfbc.nvFBCDestroyHandle(m_fbc_handle, &destroy_handle_params);
	// assert(status == NVFBC_SUCCESS); // TODO this fails
	
	dlclose(m_lib_nvfbc);
}

void capture_x11_nvfbc::do_capture()
{
	NVFBC_FRAME_GRAB_INFO grab_info{};
	
	NVFBC_TOSYS_GRAB_FRAME_PARAMS grab_params{};
	grab_params.dwVersion = NVFBC_TOSYS_GRAB_FRAME_PARAMS_VER;
	grab_params.dwFlags = NVFBC_TOSYS_GRAB_FLAGS_NOFLAGS;
	grab_params.pFrameGrabInfo = &grab_info;
	
	NVFBCSTATUS status;
	status = m_nvfbc.nvFBCToSysGrabFrame(m_fbc_handle, &grab_params);
	assert(status == NVFBC_SUCCESS);
}

void capture_x11_nvfbc::get_pixel(int x, int y, float &r, float &g, float &b)
{
	assert(x >= 0 && x < get_width());
	assert(y >= 0 && y < get_height());
	assert(m_buffer != nullptr);
	
	uint8_t *pixel = &m_buffer[3 * (x + y * get_width())];
	r = pixel[0] / 255.f;
	g = pixel[1] / 255.f;
	b = pixel[2] / 255.f;
}
