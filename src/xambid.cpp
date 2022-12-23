#include <iostream>
#include <chrono>
#include <sstream>
#include <thread>
#include <signal.h>
#include "capture_x11_nvfbc.hpp"
#include "x11_context.hpp"
#include "socket.hpp"
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

	unix_socket_server serv("./funny_socket");
	
	while (alive)
	{
		serv.update_connections();

		capture.do_capture();
		
		float r, g, b;
		capture.get_pixel(2560/2, 1440/2, r, g, b);
		
		std::ostringstream ss; 
		ss << r << " " << g << " " << b << std::endl;

		serv.broadcast(ss.str().c_str(), ss.str().length());

		std::cout << ss.str();
		
		std::this_thread::sleep_for(10ms);
	}
	
}
