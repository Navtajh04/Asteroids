#include "SmallAsteroid.hpp"

SmallAsteroid::SmallAsteroid()
{
	dx = (rand() % 3) + 0.5;
	dy = (rand() % 3) + 0.5;
	name = "small_asteroid";
}
