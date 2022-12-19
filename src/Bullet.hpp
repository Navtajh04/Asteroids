#ifndef BULLET_H
#define BULLET_H
#include "Platform/Platform.hpp"
#include "Thing.hpp"
#include <SFML/Graphics.hpp>

class Bullet : public Thing{
	public:
	  Bullet();
	  void update();
};

#endif