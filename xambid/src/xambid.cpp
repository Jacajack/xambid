#include <iostream>
#include <chrono>
#include <sstream>
#include <thread>
#include <signal.h>
#include <xambi/x11_context.hpp>
#include <xambi/capture_x11.hpp>
#include <xambi/capture_x11_shm.hpp>
#include <xambi/capture_x11_nvfbc.hpp>
#include "socket.hpp"

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
		

		float r{}, g{}, b{};
		int cnt = 0;
		for (int y = 1200; y < 1440; y++)
			for (int x = 0; x < 2560; x++)
			{
				float pr, pg, pb;
				capture.get_pixel(x, y, pr, pg, pb);
				r += pr;
				g += pg;
				b += pb;
				cnt++;
			}

		r /= cnt;
		g /= cnt;
		b /= cnt;
		
		std::ostringstream ss; 
		ss << r << " " << g << " " << b << std::endl;

		serv.broadcast(ss.str().c_str(), ss.str().length());

		std::cout << ss.str();
		
		std::this_thread::sleep_for(10ms);
	}
	
}
