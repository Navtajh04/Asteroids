#include "Bullet.hpp"
#include <cmath>
#ifndef DEGTORAD
	#define DEGTORAD 0.017453f
const int WIDTH = 1200;
const int HEIGHT = 800;
#endif

Bullet::Bullet()
{
	name = "bullet";
}

void Bullet::update()
{
	dx = (std::cos(angle * DEGTORAD) * 6);
	dy = (std::sin(angle * DEGTORAD) * 6);
	x += dx;
	y += dy;
	if ((x < 0) || (y < 0) || (x > WIDTH) || (y > HEIGHT))
	{
		alive = 0;
	}
}