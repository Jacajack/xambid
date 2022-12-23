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
#include "xambi/rgb_color.hpp"

volatile bool alive = true;

void sigint_handler(int signum)
{
	alive = false;
}

int main(int argc, char *argv[])
{
	using namespace std::chrono_literals;
	
	signal(SIGINT, sigint_handler);
	
	xambi::x11_context xctx;
	xambi::capture_x11_shm capture{xctx};

	unix_socket_server serv("./funny_socket");
	
	while (alive)
	{
		serv.update_connections();

		capture.do_capture();
		
		int w = 2560;
		int h = 1440;
		std::vector<xambi::rgb_color> pixels(w * h);
		capture.get_pixel_rect(pixels.data(), 0, 0, w, h);

		float r{}, g{}, b{};
		int cnt = 0;
		for (const auto &p : pixels)
		{
			r += p.r / 255.f;
			g += p.g / 255.f;
			b += p.b / 255.f;
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
