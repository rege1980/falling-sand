#include "game.h"

game::game(window* window, const int max_pixel_stack)
	: grid_width(static_cast<int>(window->get_width() / window->get_pixel_size()))
	  , grid_height(static_cast<int>(window->get_height() / window->get_pixel_size()))
	  , list(create_list(grid_width, grid_height))
	  , window_(window)
	  , max_pixel_stack_(max_pixel_stack)
	  , brush_size_(1)
	  , brush_type_(SAND)
{
}

void game::draw() const
{
	// draw particles

	// create list of all colors
	std::vector<SDL_Color> colors;
	for (auto& i : list)
		if (!vec_contains_color(i.color, colors))
			colors.push_back(i.color);

	const auto points = new SDL_Point[max_pixel_stack_];
	int x;
	int y;
	for (const auto color : colors)
	{
		int k = 0;
		for (int j = 0; j < static_cast<int>(list.size()); j++)
		{
			if (compare_colors(list[j].color, color))
			{
				index_to_cords(j, &x, &y);
				points[k].x = x;
				points[k].y = y;
				k++;
			}
			if (k >= max_pixel_stack_)
			{
				k = 0;
				window_->draw_pixels(points, max_pixel_stack_, color);
			}
		}
		if (k < max_pixel_stack_)
		{
			window_->draw_pixels(points, k, color);
		}
	}

	// draw cursor
	for (int i = window_->mouse_position_y - brush_size_ + 1; i < window_->mouse_position_y + brush_size_; i++)
	{
		for (int j = window_->mouse_position_x - brush_size_ + 1; j < window_->mouse_position_x + brush_size_; j++)
		{
			//if (i >= 0 && j >= 0 && j < gridW && i < grid_height)
			{
				window_->draw_pixel(j, i, particle(nullptr, NULL, brush_type_).color);
			}
		}
	}
}

void game::update()
{
	for (auto& i : list)
	{
		i.was_modified = false;
	}
	// update particles randomly from left to right or vice-versa
	// to get rid of some patterns
	if (window::random(0, 1) == 0)
	{
		for (auto& i : list)
		{
			i.update();
		}
	}
	else
	{
		for (int i = static_cast<int>(list.size()) - 1; i > 0; i--)
		{
			list[i].update();
		}
	}

	// mouse control
	if (window_->mouse_state == SDL_BUTTON_RMASK) brush(window_->mouse_position_x, window_->mouse_position_y, true);
	if (window_->mouse_state == SDL_BUTTON_LMASK) brush(window_->mouse_position_x, window_->mouse_position_y, false);
}

std::vector<particle> game::create_list(const int w, const int h)
{
	std::vector<particle> temp;
	const int size = w * h;
	temp.reserve(size);
	// creates grid filled with VOID particles
	for (int i = 0; i < w * h; i++)
	{
		temp.emplace_back(this, 0, AIR);
	}
	return temp;
}

void game::brush(const int x, const int y, const bool erase)
{
	// fill brush space
	for (int i = y - brush_size_ + 1; i < y + brush_size_; i++)
	{
		for (int j = x - brush_size_ + 1; j < x + brush_size_; j++)
		{
			if (j >= 0 && i >= 0 && j < grid_width && i < grid_height)
			{
				// set particle based on brushType or erase
				if (erase)
				{
					list[coords_to_index(j, i)] = particle(this, coords_to_index(j, i), AIR);
				}
				else if (particle::priority(brush_type_, list[coords_to_index(j, i)].type) && window::random(0, 0) == 0)
					list[coords_to_index(j, i)] = particle(this, coords_to_index(j, i), brush_type_);
			}
		}
	}
}

void game::brush_size_change(const int val)
{
	brush_size_ += val;
	if (brush_size_ < 1) brush_size_ = 20;
	if (brush_size_ > 20) brush_size_ = 1;
}

void game::brush_type_change(const int val)
{
	brush_type_ += val * 10;
	if (brush_type_ < WATER) brush_type_ = STONE;
	if (brush_type_ > STONE) brush_type_ = WATER;
}

bool game::vec_contains_color(const SDL_Color col, const std::vector<SDL_Color>& vec)
{
	return std::any_of(vec.cbegin(), vec.cend(), [col](const SDL_Color i) { return compare_colors(col, i); });
}

bool game::compare_colors(const SDL_Color col1, const SDL_Color col2)
{
	return col1.r == col2.r &&
		col1.g == col2.g &&
		col1.b == col2.b &&
		col1.a == col2.a;
}

void game::index_to_cords(const int index, int* x, int* y) const
{
	*x = index % grid_width;
	*y = index / grid_width;
}
