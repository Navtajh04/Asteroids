#include "Animate.hpp"
#include "Asteroid.hpp"
#include "Bullet.hpp"
#include "Platform/Platform.hpp"
#include "Player.hpp"
#include "SmallAsteroid.hpp"
#include <SFML/Graphics.hpp>
using namespace sf;

const int WIDTH = 1200;
const int HEIGHT = 800;

float DEGTORAD = 0.017453f;

bool collided(Thing* a, Thing* b);

bool collided(Thing* a, Thing* b)
{
	return (((b->x - a->x) * (b->x - a->x) + (b->y - a->y) * (b->y - a->y)) < ((a->R + b->R) * (a->R + b->R)));
}

int main()
{
	RenderWindow app(VideoMode(WIDTH, HEIGHT), "Asteroids!");
	app.setFramerateLimit(60);
	Texture texture_1;
	Texture texture_2;
	Texture texture_3;
	Texture texture_4;
	Texture texture_5;
	Texture texture_6;
	Texture texture_7;
	texture_1.loadFromFile("player_ship.png");
	texture_2.loadFromFile("space_background2.jpg");
	texture_3.loadFromFile("Explosion-.png");
	texture_4.loadFromFile("asteroids_spritesheet_diffuse.png");
	texture_5.loadFromFile("Bullet.png");
	texture_6.loadFromFile("game-over.jpg");
	texture_7.loadFromFile("small_asteroid.png");

	Animation sBullet(texture_5, 0, 0, 32, 64, 16, 0);

	Sprite sBackground(texture_2);
	Sprite sGameOver(texture_6);
	// Sprite sPlayer(texture_1), sBackground(texture_2);
	// sPlayer.setTextureRect(IntRect(0, 0, 40, 40));
	// sPlayer.setOrigin(20, 20);

	Animation sPlayer { texture_1, 0, 0, 40, 40, 1, 0 };

	Animation sAst { texture_4, 0, 0, 64, 64, 23, 0.15 };

	Animation sSmall { texture_7, 0, 0, 32, 32, 23, 0.15 };

	sAst.sprite.setPosition(400, 400);

	std::list<Thing*> things;

	// float x { 300 }, y = { 300 };
	// float dx { 0 }, dy { 0 }, angle { 0 };
	// bool thrust {};
	int level { 1 };
	int ammo { 15 };
	bool reloading { false };
	Clock clock;

	Player* p { new Player };
	p->settings(sPlayer, 600, 400, 0, 20);

	while (app.isOpen())
	{
		Time time = clock.getElapsedTime();
		if (things.size() == 0)
		{
			p->x = 600;
			p->y = 400;
			for (int k { 0 }; k < (2 * level); ++k)
			{
				Asteroid* a { new Asteroid() };
				a->settings(sAst, rand() % ((int)p->x - 200), rand() % ((int)p->y - 200), rand() % 360, 25);
				things.push_back(a);
				Asteroid* b { new Asteroid() };
				b->settings(sAst, (rand() % ((int)p->x - 200)) + p->x + 200, (rand() % ((int)p->y - 200)) + p->y + 200, rand() % 360, 25);
				things.push_back(b);
			}
			++level;
		}
		Event event;
		while (app.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				app.close();
			}

			if (event.type == Event::KeyPressed)
			{
				if ((event.key.code == Keyboard::Space) && (ammo > 0))
				{
					Bullet* b { new Bullet() };
					b->settings(sBullet, p->x, p->y, p->angle, 10);
					things.push_back(b);
					--ammo;
				}
			}
		}
		if (reloading && (time.asMilliseconds() > 1500))
		{
			reloading = false;
			ammo = 15;
		}

		if (((ammo == 0) || Keyboard::isKeyPressed(Keyboard::R)) && !reloading)
		{
			time = clock.restart();
			reloading = true;
			ammo = 0;
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			p->angle += 5;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			p->angle -= 5;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			p->thrust = true;
		}
		else
		{
			p->thrust = false;
		}

		for (auto a : things)
		{
			for (auto b : things)
			{
				if ((a->name == "asteroid") || (a->name == "small_asteroid"))
				{
					if (b->name == "bullet")
					{
						if (collided(a, b))
						{
							if (a->name == "asteroid")
							{
								a->alive = false;
								b->alive = false;
								SmallAsteroid* c { new SmallAsteroid() };
								c->settings(sSmall, a->x, a->y, a->angle + 90, 12);
								SmallAsteroid* d { new SmallAsteroid() };
								d->settings(sSmall, a->x, a->y, a->angle - 90, 12);
								things.push_back(c);
								things.push_back(d);
							}
							else
							{
								a->alive = false;
								b->alive = false;
							}
						}
					}
					else if (collided(a, p))
					{
						while (!(Keyboard::isKeyPressed(Keyboard::Escape)))
						{
							app.clear();
							app.draw(sGameOver);
							app.display();
						}
					}
				}
			}
		}

		// movement of the player ship
		if (p->thrust)
		{
			p->dx += std::cos(p->angle * DEGTORAD) * 0.25;
			p->dy += std::sin(p->angle * DEGTORAD) * 0.25;
		}
		else
		{
			p->dx *= 0.98;
			p->dy *= 0.98;
		}

		int max_speed { 20 };
		float speed { std::sqrt((p->dx * p->dx) + (p->dy * p->dy)) };
		if (speed > max_speed)
		{
			p->dx *= max_speed / speed;
			p->dy *= max_speed / speed;
		}

		p->x += p->dx;
		p->y += p->dy;

		if (p->x > WIDTH)
		{
			p->x = 0;
		}
		if (p->x < 0)
		{
			p->x = WIDTH;
		}
		if (p->y > HEIGHT)
		{
			p->y = 0;
		}
		if (p->y < 0)
		{
			p->y = HEIGHT;
		}

		p->ani.sprite.setPosition(p->x, p->y);
		p->ani.sprite.setRotation(p->angle + 90);

		sAst.update();

		for (auto k { things.begin() }; k != things.end();)
		{
			Thing* e { *k };
			e->update();
			e->ani.update();
			if (e->alive == false)
			{
				Animation sExplosion(texture_3, e->x, e->y, 70, 70, 5, 1);
				sExplosion.sprite.setPosition(e->x, e->y);
				for (int i { 0 }; i < 5; ++i)
				{
					app.draw(sExplosion.sprite);
					sExplosion.update();
				}
				k = things.erase(k);
				delete e;
			}
			else
			{
				++k;
			}
		}

		app.clear();
		app.draw(sBackground);
		app.draw(p->ani.sprite);
		// app.draw(sExplosion);
		// app.draw(sAst.sprite);
		for (auto i : things)
		{
			i->draw(app);
		}
		app.display();
	}
	return 0;
}
