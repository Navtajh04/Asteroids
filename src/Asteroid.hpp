#ifndef ASTEROID_H
#define ASTEROID_H
#include "Platform/Platform.hpp"
#include "Thing.hpp"
#include <SFML/Graphics.hpp>

class Asteroid : public Thing
{
public:
	Asteroid();

	virtual void update();
};
#endif