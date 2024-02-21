#pragma once

#include <vector>

#include <SDL.h>

#include "window.h"

// cross-reference fix
#ifndef GAME_H
#define GAME_H
class game;
using game = game;
#include "particle.h"

class game
{
public:
	game(window* window, int max_pixel_stack);

public:
	void draw() const;
	void update();
	void brush_size_change(int val);
	void brush_type_change(int val);

public:
	int grid_width;
	int grid_height;
	std::vector<particle> list;

private:
	std::vector<particle> create_list(int w, int h);
	void brush(int x, int y, bool erase);
	int coords_to_index(const int x, const int y) const { return y * grid_width + x; }
	void index_to_cords(int index, int* x, int* y) const;

	static bool vec_contains_color(SDL_Color col, const std::vector<SDL_Color>& vec);
	static bool compare_colors(SDL_Color col1, SDL_Color col2);

private:
	window* window_;
	int max_pixel_stack_;

	int brush_size_;
	int brush_type_;
};

#endif
