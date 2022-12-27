#include "Alien.hpp"
#include "Alien_bullet.hpp"
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
	Texture texture_8;
	Texture texture_9;

	texture_1.loadFromFile("player_ship.png");
	texture_2.loadFromFile("space_background2.jpg");
	texture_3.loadFromFile("Explosion--.png");
	texture_4.loadFromFile("asteroids_spritesheet_diffuse.png");
	texture_5.loadFromFile("Bullet.png");
	texture_6.loadFromFile("game-over.jpg");
	texture_7.loadFromFile("small_asteroid.png");
	texture_8.loadFromFile("Alien_texture.png");
	texture_9.loadFromFile("AlienBulletImage.png");

	SoundBuffer large_explosion_buffer;
	SoundBuffer small_explosion_buffer;
	SoundBuffer shooting_buffer;
	SoundBuffer alien_buffer;
	SoundBuffer thrust_buffer;

	large_explosion_buffer.loadFromFile("bangLarge.wav");
	small_explosion_buffer.loadFromFile("bangSmall.wav");
	shooting_buffer.loadFromFile("fire.wav");
	alien_buffer.loadFromFile("saucerBig.wav");
	thrust_buffer.loadFromFile("thrust.wav");

	Sound large_explosion_sound;
	Sound small_explosion_sound;
	Sound shooting_sound;
	Sound alien_sound;
	Sound thrust_sound;

	alien_sound.setVolume(28.f);
	thrust_sound.setVolume(32.f);

	large_explosion_sound.setBuffer(large_explosion_buffer);
	small_explosion_sound.setBuffer(small_explosion_buffer);
	shooting_sound.setBuffer(shooting_buffer);
	alien_sound.setBuffer(alien_buffer);
	thrust_sound.setBuffer(thrust_buffer);

	Sprite sBackground(texture_2);

	Sprite sGameOver(texture_6);

	Animation sBullet(texture_5, 0, 0, 32, 64, 16, 0);

	Animation sPlayer { texture_1, 0, 0, 40, 40, 1, 0 };

	Animation sAst { texture_4, 0, 0, 64, 64, 23, 0.15 };

	Animation sAlien { texture_8, 0, 0, 40, 40, 1, 0 };

	Animation sSmall { texture_7, 0, 0, 32, 32, 23, 0.15 };

	Animation sAlienBullet { texture_9, 0, 0, 32, 64, 16, 0 };

	Animation sExplosion(texture_3, 0, 0, 70, 70, 5, 0.1);

	std::list<Thing*> things;

	int level { 0 };
	int ammo { 15 };
	bool reloading { false };
	Clock reload_clock;
	Clock shot_clock;
	Clock spawn_clock;
	Clock alien_bullet_clock;
	Text score;
	Text ammo_text;
	Font font;
	font.loadFromFile("Roboto-Bold.ttf");
	score.setFont(font);
	ammo_text.setFont(font);
	score.setStyle(Text::Bold);
	ammo_text.setStyle(Text::Bold);
	score.setCharacterSize(35);
	ammo_text.setCharacterSize(35);
	score.setPosition(20, 0);
	ammo_text.setPosition(820, 0);

	Player* p { new Player };
	p->settings(sPlayer, 600, 400, 0, 20);

	bool alien_alive { false };

	Time time_reload;
	Time time_shot;
	Time time_spawn;
	Time time_alien_bullet;

	while (app.isOpen())
	{
		if (things.size() == 0)
		{
			p->x = 600;
			p->y = 400;
			for (int k { 0 }; k < (level + 1); ++k)
			{
				Asteroid* a { new Asteroid() };
				a->settings(sAst, rand() % ((int)p->x - 200), rand() % ((int)p->y - 200), rand() % 360, 25);
				things.push_back(a);
				Asteroid* b { new Asteroid() };
				b->settings(sAst, (rand() % ((int)p->x - 200)) + p->x + 200, (rand() % ((int)p->y - 200)) + p->y + 200, rand() % 360, 25);
				things.push_back(b);
			}
			++level;
			spawn_clock.restart();
		}
		time_reload = reload_clock.getElapsedTime();
		time_shot = shot_clock.getElapsedTime();
		time_spawn = spawn_clock.getElapsedTime();
		time_alien_bullet = alien_bullet_clock.getElapsedTime();

		if ((time_spawn.asSeconds() > 15) && !alien_alive && (level > 2))
		{
			Alien* a { new Alien() };
			a->settings(sAlien, a->x, a->y, a->angle, 20);
			things.push_back(a);
			spawn_clock.restart();
			alien_alive = true;
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
				if ((event.key.code == Keyboard::Space) && (ammo > 0) && (time_shot.asMilliseconds() > 250))
				{
					Bullet* b { new Bullet() };
					b->settings(sBullet, p->x, p->y, p->angle, 10);
					things.push_back(b);
					--ammo;
					shooting_sound.play();
					shot_clock.restart();
				}
			}
		}
		if (reloading && (time_reload.asMilliseconds() > 1500))
		{
			reloading = false;
			ammo = 15;
		}

		if (((ammo == 0) || Keyboard::isKeyPressed(Keyboard::R)) && !reloading)
		{
			time_reload = reload_clock.restart();
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
			thrust_sound.play();
		}
		else
		{
			p->thrust = false;
		}

		for (auto a : things)
		{
			for (auto b : things)
			{
				if ((a->name == "asteroid") || (a->name == "small_asteroid") || (a->name == "alien") || (a->name == "alien_bullet"))
				{
					if (collided(a, b))
					{
						if (b->name == "bullet")
						{
							if (a->name == "asteroid")
							{
								a->alive = false;
								b->alive = false;
								large_explosion_sound.play();
								SmallAsteroid* c { new SmallAsteroid() };
								c->settings(sSmall, a->x, a->y, a->angle + 90, 12);
								SmallAsteroid* d { new SmallAsteroid() };
								d->settings(sSmall, a->x, a->y, a->angle - 90, 12);
								things.push_back(c);
								things.push_back(d);
							}
							else if (a->name == "small_asteroid")
							{
								small_explosion_sound.play();
								a->alive = false;
								b->alive = false;
							}
							else if (a->name == "alien")
							{
								a->alive = false;
								b->alive = false;
								alien_alive = false;
								spawn_clock.restart();
							}
						}
						else if (((a->name == "asteroid") || (a->name == "small_asteroid")) && (b->name == "alien_bullet"))
						{
							b->alive = false;
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

		for (auto k { things.begin() }; k != things.end();)
		{
			Thing* e { *k };
			e->update();
			e->ani.update();
			if (e->alive == false)
			{
				if ((e->name == "explosion") || (e->name == "bullet") || (e->name == "alien_bullet"))
				{
					k = things.erase(k);
					delete e;
				}
				else
				{
					Thing* a { new Thing() };
					a->settings(sExplosion, e->x, e->y, 0, 5);
					a->name = "explosion";
					things.push_back(a);
					k = things.erase(k);
					delete e;
				}
			}
			else
			{
				++k;
			}
			if ((e->name == "alien") && (time_alien_bullet.asMilliseconds() > 1750))
			{
				AlienBullet* b { new AlienBullet() };
				float shooting_angle;
				if (e->x < p->x)
				{
					shooting_angle = atan((p->y - e->y) / (p->x - e->x));
				}
				else
				{
					shooting_angle = 3.14159265358979323846 + atan((p->y - e->y) / (p->x - e->x));
				}
				b->settings(sAlienBullet, e->x + 20, e->y + 20, shooting_angle * 180 / 3.14159265358979323846, 10);
				things.push_back(b);
				alien_bullet_clock.restart();
			}
		}
		app.clear();
		app.draw(sBackground);
		app.draw(p->ani.sprite);
		for (auto i : things)
		{
			i->draw(app);
			if ((i->name == "explosion") && (i->ani.frame > 4))
			{
				i->alive = false;
			}
			else if (i->name == "explosion")
			{
				i->ani.update();
			}
			if (i->name == "alien")
			{
				alien_sound.play();
			}
		}
		score.setString("Level: " + std::to_string(level));
		ammo_text.setString("Ammo Remaining: " + std::to_string(ammo));
		app.draw(score);
		app.draw(ammo_text);
		app.display();
	}
	return 0;
}
