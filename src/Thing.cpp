#include "Thing.hpp"
#include "Platform/Platform.hpp"
#include <SFML/Graphics.hpp>

Thing::Thing() :
	alive { 1 }
{}

void Thing::settings(Animation& anim, int X, int Y, float Angle, int radius)
{
	x = X;
	y = Y;
	ani = anim;
	angle = Angle;
	R = radius;
}

void Thing::draw(RenderWindow& app)
{
	ani.sprite.setPosition(x, y);
	ani.sprite.setRotation(angle + 90);
	app.draw(ani.sprite);
}