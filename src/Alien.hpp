#include "Platform/Platform.hpp"
#include "Thing.hpp"
#include <SFML/Graphics.hpp>

class Alien : public Thing
{
public:
	Alien();

	void update();

	float target_x;
	float target_y;
};