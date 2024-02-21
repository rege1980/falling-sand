#include "window.h"

window::window(const char* title, const int position_x, const int position_y, const int width, const int height,
               const float pixel_size, const bool fullscreen, const bool resizable, const int frame_cap)
	: title_(title)
	  , position_x_(position_x)
	  , position_y_(position_y)
	  , width_(width)
	  , height_(height)
	  , fullscreen_(fullscreen)
	  , resizable_(resizable)
	  , pixel_size_(pixel_size)
	  , frame_cap_(frame_cap)
{
	running = false;
	initialization_success = true;
	delta_time = 0;
	keyboard_state = nullptr;
	last_time = 0;
	mouse_state = 0;
	mouse_position_x = 0;
	mouse_position_y = 0;
}

void window::initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL_Error: %s", SDL_GetError());
		initialization_success = false;
	}
	else
	{
		// CREATE WINDOW
		window_ = SDL_CreateWindow(title_, position_x_, position_y_, width_, height_, 0);
		if (window_ == nullptr)
		{
			printf("SDL_Error: %s", SDL_GetError());
			initialization_success = false;
		}
		else
		{
			// WINDOW FLAGS
			if (fullscreen_) SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);
			if (resizable_) SDL_SetWindowResizable(window_, SDL_TRUE);

			// CREATE RENDERER
			renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED); //SDL_RENDERER_ACCELERATED

			if (renderer_ == nullptr)
			{
				printf("SDL_Error: %s", SDL_GetError());
				initialization_success = false;
			}
			else
			{
				// RENDERER FLAGS
				SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest"); // "linear"
				SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
				SDL_RenderSetScale(renderer_, static_cast<float>(pixel_size_), static_cast<float>(pixel_size_));
			}
		}
	}
	if (initialization_success) running = true;
	srand(SDL_GetTicks());
}

void window::close() const
{
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);

	SDL_Quit();
}

void window::clear(const SDL_Color color) const
{
	SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
	SDL_RenderClear(renderer_);
	SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
}

void window::present() const
{
	SDL_SetRenderTarget(renderer_, nullptr);
	SDL_RenderPresent(renderer_);
	//printf("%f\n", 1 / delta_time);
}

void window::update()
{
	delta_time = static_cast<double>(SDL_GetTicks64() - last_time) / 1000.0;
	last_time = SDL_GetTicks64();

	keyboard_state = SDL_GetKeyboardState(nullptr);

	mouse_state = SDL_GetMouseState(&mouse_position_x, &mouse_position_y);


	if (mouse_position_x < 0) mouse_position_x = 0;
	if (mouse_position_y < 0) mouse_position_y = 0;
	if (mouse_position_x >= width_) mouse_position_x = width_ - 1;
	if (mouse_position_y >= height_) mouse_position_y = height_ - 1;

	mouse_position_x /= static_cast<int>(pixel_size_);
	mouse_position_y /= static_cast<int>(pixel_size_);

	//printf("\n[%i] Dt:%.4f", SDL_GetTicks(), delta_time);
}

int window::random(const int min, const int max)
{
	return rand() % (max - min + 1) + min; // NOLINT(concurrency-mt-unsafe)
}

void window::draw_pixel(const int x, const int y, const SDL_Color color) const
{
	SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(renderer_, x, y);
	SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
}

void window::draw_pixels(const SDL_Point* points, const int count, const SDL_Color color) const
{
	SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoints(renderer_, points, count);
	SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
}
