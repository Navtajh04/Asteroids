#include "Platform/Platform.hpp"
#include "Thing.hpp"
#include <SFML/Graphics.hpp>

class Player : public Thing{
	public:
	  Player();

	  void update();

	  bool thrust;

};