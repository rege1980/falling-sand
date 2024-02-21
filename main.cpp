#include <cstdio>
#include <iostream>
#include <map>
#include <SDL.h>
#include <string>

#include "config.h"
#include "game.h"
#include "window.h"

int main(int argc, char* argv[])
{
	// default values
	const auto title = "Falling sand";
	constexpr int window_pos_x = SDL_WINDOWPOS_UNDEFINED;
	constexpr int window_pos_y = SDL_WINDOWPOS_UNDEFINED;
	int window_width = 1280;
	int window_height = 720;
	int pixel_size = 10;
	int fullscreen = false;
	constexpr bool resizable = false;
	int frame_cap = 30;
	int max_pixel_stack = 1000;
	const auto cfg_path = "\config.cfg"; // NOLINT(clang-diagnostic-unknown-escape-sequence)


	// read values from config file
	std::map<std::string, int*> values;
	values.try_emplace("window_width", &window_width);
	values.try_emplace("window_height", &window_height);
	values.try_emplace("pixel_size", &pixel_size);
	values.try_emplace("fullscreen", &fullscreen);
	values.try_emplace("frame_cap", &frame_cap);
	values.try_emplace("max_pixel_stack", &max_pixel_stack);
	if (!load_cfg(cfg_path, &values))
	{
		printf("Config error");
	}

	window window(title, window_pos_x, window_pos_y, window_width, window_height, static_cast<float>(pixel_size),
	              static_cast<bool>(fullscreen), resizable, frame_cap);
	window.initialize();

	SDL_Event event;

	game game(&window, max_pixel_stack);

	if (window.initialization_success)
	{
		while (window.running)
		{
			// input control
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					window.running = false;
				if (event.type == SDL_KEYUP)
				{
					if (event.key.keysym.sym == SDLK_ESCAPE)
						window.running = false;

					// game brush type
					if (event.key.keysym.sym == SDLK_w)
						game.brush_type_change(1);
					if (event.key.keysym.sym == SDLK_s)
						game.brush_type_change(-1);
					// frame cap
					if (event.key.keysym.sym == SDLK_KP_MINUS)
					{
						if (window.get_frame_cap() > 5) window.set_frame_cap(window.get_frame_cap() - 1);
						printf("\n""frameCap=%i", window.get_frame_cap());
					}
					if (event.key.keysym.sym == SDLK_KP_PLUS)
					{
						if (window.get_frame_cap() < 200) window.set_frame_cap(window.get_frame_cap() + 1);
						printf("\n""frameCap=%i", window.get_frame_cap());
					}
				}
				// mouse wheel
				if (event.type == SDL_MOUSEWHEEL)
				{
					game.brush_size_change(event.wheel.y);
				}
			}

			// calculate times
			const int frame_delay = static_cast<int>(1000.0 / window.get_frame_cap());
			const int time_left = static_cast<int>(frame_delay - (SDL_GetTicks64() - window.last_time));

			if (time_left <= 0)
			{
				// main loop
				window.update();

				game.update();

				game.draw();

				window.present();
			}
			else
				SDL_Delay(time_left / 10);
		}
	}

	window.close();

	return 0;
}
