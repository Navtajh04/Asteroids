#include "SmallAsteroid.hpp"

SmallAsteroid::SmallAsteroid()
{
	dx = rand() % 3;
	dy = rand() % 3;
	name = "small_asteroid";
}