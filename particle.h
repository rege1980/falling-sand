#pragma once

#include <SDL.h>

// cross-reference fix
#ifndef PARTICLE_H
#define PARTICLE_H
class particle;
using particle = particle;
#include "game.h"

enum
{
	VOID = 0,
	AIR = 10,
	WATER = 20,
	SAND = 30,
	STONE = 40
};

enum
{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3
};

class game;

class particle
{
public:
	particle(game* game, int id, int type);

public:
	void update();

	static bool priority(int a, int b);

public:
	SDL_Color color;
	int type;

	bool was_modified; // already modified this frame
	bool active; // to be modified further

private:
	bool down_lr_physics();
	bool up_lr_physics();
	bool lr_physics() const;
	void rand_swap(int probability) const;

	void swap_particles(int id) const;
	int get_index_for(int dir, int index) const;

	//Particle 

private:
	game* game_;

	int id_;
	float velocity_;
	float acceleration_;
	float max_velocity_;
};

#endif
