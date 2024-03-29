#pragma once
#include "capture_backend.hpp"
#include "utils.hpp"
#include "x11_context.hpp"
#include "nv/NvFBC.h"
#include <vector>

namespace xambi {

class capture_x11_nvfbc : public capture_backend, public no_copy, public no_move
{
public:
	explicit capture_x11_nvfbc(x11_context &xctx);
	~capture_x11_nvfbc() override;
	
	int get_width() const override {return m_width;}
	int get_height() const override {return m_height;}
	void do_capture() override;
	void get_pixel_rect(rgb_color *dest, int x0, int y0, int w, int h) override;

private:
	x11_context *m_xctx;
	void *m_lib_nvfbc;
	NVFBC_API_FUNCTION_LIST m_nvfbc;
	NVFBC_SESSION_HANDLE m_fbc_handle;
	uint8_t *m_buffer;
	int m_width;
	int m_height;
};

}