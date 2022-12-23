#include <iostream>
#include <chrono>
#include <ratio>
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
#include <xambi/averaging_color_aggregate.hpp>
#include <xambi/color_mode_aggregate.hpp>
#include <xambi/rgb_color.hpp>
#include <xambi/color_filter.hpp>
#include <xambi/lowpass_color_filter.hpp>
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
	xambi::capture_x11_nvfbc capture{xctx};
	xambi::screen_rect rect{0, 0, 2560, 1440};
	xambi::random_screen_sampler sampler{capture, rect, rng, 0.1f};
	xambi::color_mode_aggregate<4, 4, 4> aggregate;
	// xambi::averaging_color_aggregate aggregate;
	// xambi::passthrough_color_filter filter;
	xambi::lowpass_color_filter filter{0.5f};

	unix_socket_server serv("./funny_socket");
	
	auto last_t = std::chrono::high_resolution_clock::now();
	while (alive)
	{
		auto t = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> dt_dur{t - last_t};
		double dt = dt_dur.count();

		// std::cerr << dt << std::endl;

		serv.update_connections();
		capture.do_capture();

		auto pixels = sampler.sample();
		auto icolor = aggregate.process(pixels.data(), pixels.size());
		auto color = filter.feed(icolor, dt);

		float r = color.r / 255.f;
		float g = color.g / 255.f;
		float b = color.b / 255.f;

		std::ostringstream ss; 
		ss << r << " " << g << " " << b << std::endl;

		serv.broadcast(ss.str().c_str(), ss.str().length());

		std::cout << ss.str();
		
		// std::this_thread::sleep_for(10ms);
		last_t = t;
	}
	
}
