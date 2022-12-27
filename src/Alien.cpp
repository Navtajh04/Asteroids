#define _USE_MATH_DEFINES
#include "Alien.hpp"
#include <cmath>
#include <iostream>

const double pi = 3.14159265358979323846;

Alien::Alien()
{
	if (rand() % 2)
	{
		x = 0;
	}
	else
	{
		x = 1200;
	}
	y = rand() % 800;
	target_x = rand() % 1200;
	target_y = rand() % 800;
	if (x < target_x)
	{
		angle = atan((target_y - y) / (target_x - x));
	}
	else
	{
		angle = pi + atan((target_y - y) / (target_x - x));
	}
	dx = std::cos(angle) * 1.5;
	dy = std::sin(angle) * 1.5;
	name = "alien";
}

void Alien::update()
{
	if (((std::abs(target_x - x) < 5) && (std::abs(target_y - y) < 5)))
	{
		target_x = rand() % 1200;
		target_y = rand() % 800;
		if (x < target_x)
		{
			angle = atan((target_y - y) / (target_x - x));
		}
		else
		{
			angle = pi + atan((target_y - y) / (target_x - x));
		}
		dx = std::cos(angle) * 1.5;
		dy = std::sin(angle) * 1.5;
	}
	x += dx;
	y += dy;
}