#include <iostream>
#include <chrono>
#include <sstream>
#include <thread>
#include <random>
#include <signal.h>
#include <xambi/x11_context.hpp>
#include <xambi/capture_x11.hpp>
#include <xambi/capture_x11_shm.hpp>
#include <xambi/capture_x11_nvfbc.hpp>
#include <xambi/screen_rect.hpp>
#include <xambi/naive_screen_sampler.hpp>
#include <xambi/random_screen_sampler.hpp>
#include <xambi/rgb_color.hpp>
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
	
	std::mt19937 rng{std::random_device{}()};

	xambi::x11_context xctx;
	xambi::capture_x11_shm capture{xctx};
	xambi::screen_rect rect{0, 0, 2560, 1440};
	xambi::random_screen_sampler sampler{capture, rect, rng, 0.1f};

	unix_socket_server serv("./funny_socket");
	
	while (alive)
	{
		serv.update_connections();
		capture.do_capture();

		auto pixels = sampler.sample();
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
		
		// std::this_thread::sleep_for(10ms);
	}
	
}
