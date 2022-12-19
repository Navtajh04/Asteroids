#include "Asteroid.hpp"

Asteroid::Asteroid()
{
	dx = rand() % 8 - 4;
	dy = rand() % 8 - 4;
	name = "asteroid";
}

void Asteroid::update()
{
	x += dx;
	y += dy;
	if (x > 1200)
	{
		x = 0;
	}
	if (x < 0)
	{
		x = 1200;
	}
	if (y > 800)
	{
		y = 0;
	}
	if (y < 0)
	{
		y = 800;
	}
}