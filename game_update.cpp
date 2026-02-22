#include "includes/game.hpp"
#include "includes/utilities.hpp"

void Game::updateBullets()
{
	// Proiettili del giocatore
	for (size_t i = 0; i < p_bullets.size();)
	{
		Bullet &b = p_bullets[i];
		b.tick++;
		if (b.tick >= b.speed)
		{
			b.setY(b.getY() + b.direction);
			b.tick = 0;
		}
		if (b.getY() <= 0 || b.getY() >= win_height - 1)
		{
			p_bullets.erase(p_bullets.begin() + i);
		}
		else
		{
			i++;
		}
	}

	// Proiettili dei nemici
	for (size_t i = 0; i < e_bullets.size();)
	{
		Bullet &b = e_bullets[i];
		b.tick++;
		if (b.tick >= b.speed)
		{
			b.setY(b.getY() + b.direction);
			b.tick = 0;
		}
		if (b.getY() <= 0 || b.getY() >= win_height - 1)
		{
			e_bullets.erase(e_bullets.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

void Game::updateEnemies()
{
	for (size_t i = 0; i < enemies.size();)
	{
		Enemy &e = enemies[i];
		int newX = e.getX() + RandomBetweenInt(-3, 3);

		// Mantieni i nemici dentro lo schermo
		if (newX < 2)
			newX = 2;
		if (newX > win_width - 3)
			newX = win_width - 3;

		e.setX(newX);
		e.setY(e.getY() + 1);

		e.update();

		if (e.getY() >= win_height - 1)
		{
			enemies.erase(enemies.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

void Game::updateAsteroids()
{
	for (size_t i = 0; i < asteroids.size();)
	{
		Asteroid &a = asteroids[i];
		a.setY(a.getY() + 1);
		if (a.getY() >= win_height - 1)
		{
			asteroids.erase(asteroids.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

void Game::checkCollisions()
{
	for (size_t i = 0; i < p_bullets.size();)
	{
		bool hit = false;
		for (size_t j = 0; j < enemies.size(); j++)
		{
			int ex = enemies[j].getX();
			int ey = enemies[j].getY();
			int bx = p_bullets[i].getX();
			int by = p_bullets[i].getY();

			// Nemico occupa: (ex-1, ey), (ex, ey), (ex+1, ey)
			if (by == ey && (bx == ex - 1 || bx == ex || bx == ex + 1))
			{
				if (enemies[j].getHealth() > 1)
					enemies[j].setHealth(enemies[j].getHealth() - 1);
				else
				{
					enemies.erase(enemies.begin() + j);
					hit = true;
					score += 10;
					break;
				}
			}
		}
		if (hit)
			p_bullets.erase(p_bullets.begin() + i);
		else
			i++;
	}

	for (size_t i = 0; i < p_bullets.size();)
	{
		bool hit = false;
		for (size_t j = 0; j < asteroids.size(); j++)
		{
			int ax = asteroids[j].getX();
			int ay = asteroids[j].getY();
			int bx = p_bullets[i].getX();
			int by = p_bullets[i].getY();

			if (by == ay && bx == ax)
			{
				asteroids.erase(asteroids.begin() + j);
				hit = true;
				score += 5;
				break;
			}
		}
		if (hit)
			p_bullets.erase(p_bullets.begin() + i);
		else
			i++;
	}

	for (size_t i = 0; i < e_bullets.size();)
	{
		int bx = e_bullets[i].getX();
		int by = e_bullets[i].getY();
		int px = player->getX();
		int py = player->getY();

		if ((by == py && bx == px) || (by == py + 1 && bx >= px - 1 && bx <= px + 1))
		{
			player->loseLife(10);
			e_bullets.erase(e_bullets.begin() + i);
		}
		else
		{
			i++;
		}
	}

	for (size_t i = 0; i < asteroids.size();)
	{
		int ax = asteroids[i].getX();
		int ay = asteroids[i].getY();
		int px = player->getX();
		int py = player->getY();

		if ((ay == py && ax == px) || (ay == py + 1 && ax >= px - 1 && ax <= px + 1))
		{
			player->die();
			asteroids.erase(asteroids.begin() + i);
		}
		else
		{
			i++;
		}
	}

	for (size_t i = 0; i < enemies.size();)
	{
		int ex = enemies[i].getX();
		int ey = enemies[i].getY();
		int px = player->getX();
		int py = player->getY();

		if ((ey == py && (ex == px - 1 || ex == px || ex == px + 1)) ||
			(ey == py + 1 && ex >= px - 1 && ex <= px + 1))
		{
			player->die();
			enemies.erase(enemies.begin() + i);
		}
		else
		{
			i++;
		}
	}
}
