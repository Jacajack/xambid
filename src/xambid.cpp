#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>
#include "capture_x11_nvfbc.hpp"
#include "x11_context.hpp"
// #include "capture_x11.hpp"
// #include "capture_x11_shm.hpp"

volatile bool alive = true;

void sigint_handler(int signum)
{
	alive = false;
}

int main(int argc, char *argv[])
{
	using namespace std::chrono_literals;
	
	signal(SIGINT, sigint_handler);
	
	x11_context xctx;
	capture_x11_nvfbc capture{xctx};
	
	while (alive)
	{
		capture.do_capture();
		
		float r, g, b;
		capture.get_pixel(2560/2, 1440/2, r, g, b);
		
		std::cout << r << " " << g << " " << b << std::endl;
		
		std::this_thread::sleep_for(10ms);
	}
	
}
