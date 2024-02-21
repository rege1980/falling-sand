#include "particle.h"

#include <cmath>

particle::particle(game* game, const int id, const int type)
	: type(type)
	  , was_modified(false)
	  , active(true)
	  , game_(game)
	  , id_(id)
	  , velocity_(0)

{
	color.a = 255;

	Uint8 mod = static_cast<Uint8>(window::random(-1, 1));
	mod *= 5;

	if (type == AIR)
	{
		color.r = 128 + mod;
		color.g = 128 + mod;
		color.b = 255;
		max_velocity_ = 5;
		acceleration_ = 0.6f;
	}
	else if (type == WATER)
	{
		color.r = 10 + mod;
		color.g = 10 + mod;
		color.b = 250;
		max_velocity_ = 3;
		acceleration_ = 0.4f;
	}
	else if (type == SAND)
	{
		color.r = 255;
		color.g = 255;
		color.b = 130 + mod * 2;
		max_velocity_ = 2;
		acceleration_ = 0.2f;
	}
	else if (type == STONE)
	{
		color.r = 50 + mod;
		color.g = 50 + mod;
		color.b = 50 + mod;
		max_velocity_ = 0;
		acceleration_ = 0;
	}
	else // VOID
	{
		color.r = 0;
		color.g = 0;
		color.b = 0;
		max_velocity_ = 0;
		acceleration_ = 0;
		active = false;
	}
}

void particle::update()
{
	if (!active || was_modified) return;
	was_modified = true;

	if (type == AIR)
	{
		if (!up_lr_physics())
			if (!lr_physics())
				rand_swap(10);
	}
	if (type == WATER)
	{
		if (!down_lr_physics())
			if (!lr_physics())
				rand_swap(5);
	}
	if (type == SAND)
	{
		down_lr_physics();
	}
}

bool particle::down_lr_physics()
{
	int down = get_index_for(DOWN, id_);
	if (down >= 0)
	{
		velocity_ += acceleration_;
		if (velocity_ > max_velocity_) velocity_ = max_velocity_;
		// randomize velocity if it isn't integer
		int times;
		if (window::random(0, 1)) times = static_cast<int>(floor(velocity_));
		else times = static_cast<int>(ceil(velocity_));

		// check if this particle has priority compared to the lower one
		if (priority(type, game_->list.at(down).type))
		{
			for (int i = 0; i < times; i++)
			{
				// check every step if there is free space and no bounds
				if (get_index_for(DOWN, down) >= 0)
				{
					if (!priority(type, game_->list[get_index_for(DOWN, down)].type))
					{
						break;
					}
					down = get_index_for(DOWN, down);
				}
			}
			// move pixel down
			swap_particles(down);
			return true;
		}
		const int down_r = get_index_for(RIGHT, down);
		if (down_r >= 0)
			if (priority(type, game_->list.at(down_r).type) && window::random(0, 1)) // 50/50 chance to fall down-right
			{
				swap_particles(down_r);
				return true;
			}
		const int down_l = get_index_for(LEFT, down);
		if (down_l >= 0)
			if (priority(type, game_->list.at(down_l).type) && window::random(0, 1)) // 50/50 chance to fall down-right
			{
				swap_particles(down_l);
				return true;
			}
	}
	// pixel stayed in the same place
	velocity_ = 0;
	swap_particles(id_);
	return false;
}

bool particle::up_lr_physics()
{
	int up = get_index_for(UP, id_);
	if (up >= 0)
	{
		velocity_ += acceleration_;
		if (velocity_ > max_velocity_) velocity_ = max_velocity_;
		// randomize velocity if it isn't integer
		int times;
		if (window::random(0, 1)) times = static_cast<int>(floor(velocity_));
		else times = static_cast<int>(ceil(velocity_));

		// check if this particle has priority compared to the lower one
		if (priority(type, game_->list.at(up).type))
		{
			for (int i = 0; i < times; i++)
			{
				// check every step if there is free space and no bounds
				if (get_index_for(UP, up) >= 0)
				{
					if (!priority(type, game_->list[get_index_for(UP, up)].type))
					{
						break;
					}
					up = get_index_for(UP, up);
				}
			}
			// move pixel down
			swap_particles(up);
			return true;
		}
		const int up_r = get_index_for(RIGHT, up);
		if (up_r >= 0)
			if (priority(type, game_->list.at(up_r).type) && window::random(0, 1)) // 50/50 chance to fall down-right
			{
				swap_particles(up_r);
				return true;
			}
		const int up_l = get_index_for(LEFT, up);
		if (up_l >= 0)
			if (priority(type, game_->list.at(up_l).type) && window::random(0, 1)) // 50/50 chance to fall down-right
			{
				swap_particles(up_l);
				return true;
			}
	}
	// pixel stayed in the same place
	velocity_ = 0;
	swap_particles(id_);
	return false;
}

bool particle::lr_physics() const
{
	const int left = get_index_for(LEFT, id_);
	if (left >= 0)
	{
		if (priority(type, game_->list[left].type) && window::random(0, 1))
		{
			swap_particles(left);
			return true;
		}
	}

	const int right = get_index_for(RIGHT, id_);
	if (right >= 0)
	{
		if (left >= 0 && priority(type, game_->list[right].type) && window::random(0, 1))
		{
			swap_particles(right);
			return true;
		}
	}
	swap_particles(id_);
	return false;
}

void particle::rand_swap(const int probability) const
{
	if (window::random(0, 99) < probability)
	{
		const int r = window::random(0, 3);
		const int i = get_index_for(r, id_);
		if (i >= 0 && type == game_->list[i].type)
		{
			swap_particles(i);
		}
	}
}

void particle::swap_particles(const int id) const
{
	particle temp = game_->list[id];
	game_->list[id] = *this;
	game_->list[id].id_ = id;
	temp.id_ = id_;
	game_->list[id_] = temp;
}

bool particle::priority(const int a, const int b)
{
	return a > b;
}

int particle::get_index_for(const int dir, int index) const
{
	// bound checks here
	if (dir == LEFT && ((index % game_->grid_width) - 1) >= 0)
		return --index;
	if (dir == RIGHT && ((index % game_->grid_width) + 1) < game_->grid_width)
		return ++index;
	if (dir == DOWN && index + game_->grid_width < static_cast<int>(game_->list.size()))
		return index + game_->grid_width;
	if (dir == UP && index - game_->grid_width >= 0)
		return index - game_->grid_width;
	return -1;
}
