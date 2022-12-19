#ifndef ANIMATE_H
#define ANIMATE_H
#include "Platform/Platform.hpp"
#include <SFML/Graphics.hpp>
using namespace sf;

class Animation
{
public:
	float frame, speed;
	Sprite sprite;
	std::vector<IntRect> frames;

	Animation()
	{}

	Animation(Texture& t, int x, int y, int w, int h, int count, float Speed)
	{
		frame = 0;
		speed = Speed;

		for (int i { 0 }; i < count; ++i)
		{
			frames.push_back(IntRect(x + i * w, y, w, h));
		}

		sprite.setTexture(t);
		sprite.setOrigin(w / 2, h / 2);
		sprite.setTextureRect(frames[0]);
	}

	void update()
	{
		frame += speed;
		int n = frames.size();
		if (frame >= n)
		{
			frame -= n;
		}
		if (n > 0)
		{
			sprite.setTextureRect(frames[int(frame)]);
		}
	}
};

#endif