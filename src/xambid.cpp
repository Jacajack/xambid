#include <iostream>
#include <chrono>
#include <thread>
#include "x11_context.hpp"
// #include "capture_x11.hpp"
#include "capture_x11_shm.hpp"

int main(int argc, char *argv[])
{
	using namespace std::chrono_literals;
	
	x11_context xctx;
	capture_x11_shm capture{xctx};
	
	while (1)
	{
		capture.do_capture();
		
		float r, g, b;
		capture.get_pixel(2560/2, 1440/2, r, g, b);
		
		std::cout << r << " " << g << " " << b << std::endl;
		
		std::this_thread::sleep_for(100ms);
	}
	
}
