#ifndef THING_H
#define THING_H
#include "Animate.hpp"
#include "Platform/Platform.hpp"
#include <SFML/Graphics.hpp>

class Thing
{
public:
	float x, y, dx, dy, R, angle;
	bool alive;
	std::string name;
	Animation ani;

	Thing();
	virtual ~Thing()
	{}

	void settings(Animation& anim, int X, int Y, float Angle = 0, int radius = 1);

	virtual void update() {};

	void draw(RenderWindow& app);
};

#endif