#include "Player.hpp"

Player::Player(){
	x = 300;
	y = 300;
	dx = 0;
	dy = 0;
	angle = 0;
	thrust = false;
}
void Player::update()
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