#pragma once

#include <SDL.h>
#include <cstdio>
#include <random>

class window
{
public:
	// CONSTRUCTOR
	window(const char* title, int position_x, int position_y, int width, int height, float pixel_size, bool fullscreen,
	       bool resizable, int frame_cap);

	// PUBLIC VARIABLES
	bool initialization_success;
	bool running;
	double delta_time;
	Uint64 last_time;
	const Uint8* keyboard_state;
	Uint64 mouse_state;
	int mouse_position_x;
	int mouse_position_y;

	// PUBLIC FUNCTIONS
	void initialize();
	void close() const;
	void clear(SDL_Color color) const;
	void present() const;
	void update();
	static int random(int min, int max);

	void draw_pixel(int x, int y, SDL_Color color) const;
	void draw_pixels(const SDL_Point* points, int count, SDL_Color color) const;

	// GETTERS
	double get_pixel_size() const { return pixel_size_; }
	int get_frame_cap() const { return frame_cap_; }
	int get_width() const { return width_; }
	int get_height() const { return height_; }

	// SETTERS
	void set_frame_cap(const int cap) { frame_cap_ = cap; }

private:
	const char* title_;
	int position_x_;
	int position_y_;
	int width_;
	int height_;
	bool fullscreen_;
	bool resizable_;
	double pixel_size_;
	int frame_cap_;

	SDL_Window* window_ = nullptr;
	SDL_Renderer* renderer_ = nullptr;
};
