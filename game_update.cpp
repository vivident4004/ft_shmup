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

		if (e.getIsBoss())
			newX = e.getX(); // Il boss non si muove lateralmente



		e.setX(newX);

		if (!e.getIsBoss())
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


// Controlla se un punto (bx,by) tocca il giocatore (forma A + /|\)
static bool hitsPlayer(int bx, int by, int px, int py)
{
	return (by == py && bx == px) ||
		   (by == py + 1 && bx >= px - 1 && bx <= px + 1);
}

// Controlla se un punto (bx,by) tocca un nemico (forma (O), largo 3)
static bool hitsEnemy(int bx, int by, int ex, int ey)
{
	return (by == ey && bx >= ex - 1 && bx <= ex + 1);
}

void Game::checkCollisions()
{
	// Proiettili giocatore vs nemici
	for (size_t i = 0; i < p_bullets.size();)
	{
		bool consumed = false;
		for (size_t j = 0; j < enemies.size(); j++)
		{
			int ex = enemies[j].getX();
			int ey = enemies[j].getY();
			int bx = p_bullets[i].getX();
			int by = p_bullets[i].getY();

			// Nemico occupa: (ex-1, ey), (ex, ey), (ex+1, ey)
			if (by == ey && (bx == ex - 1 || bx == ex || bx == ex + 1))
			{
				enemies[j].setHealth(enemies[j].getHealth() - p_bullets[i].damage);
				if (enemies[j].getHealth() <= 0)
				{
					enemies.erase(enemies.begin() + j);
					score += 10;
				}
				consumed = true;
				break;
			}
		}
		if (consumed)
			p_bullets.erase(p_bullets.begin() + i);
		else
			i++;
	}

	// Proiettili giocatore vs asteroidi
	for (size_t i = 0; i < p_bullets.size();)
	{
		bool consumed = false;
		for (size_t j = 0; j < asteroids.size(); j++)
		{
			if (p_bullets[i].getX() == asteroids[j].getX() &&
				p_bullets[i].getY() == asteroids[j].getY())
			{
				asteroids[j].setHealth(asteroids[j].getHealth() - p_bullets[i].damage);
				if (asteroids[j].getHealth() <= 0)
				{
					asteroids.erase(asteroids.begin() + j);
					score += 5;
				}
				consumed = true;
				break;
			}
		}
		if (consumed)
			p_bullets.erase(p_bullets.begin() + i);
		else
			i++;
	}

	// Proiettili nemici vs giocatore
	int px = player->getX();
	int py = player->getY();

	for (size_t i = 0; i < e_bullets.size();)
	{
		if (hitsPlayer(e_bullets[i].getX(), e_bullets[i].getY(), px, py))
		{
			player->loseLife(10);
			e_bullets.erase(e_bullets.begin() + i);
			px = player->getX();
			py = player->getY();
		}
		else
			i++;
	}

	// Asteroidi vs giocatore
	for (size_t i = 0; i < asteroids.size();)
	{
		if (hitsPlayer(asteroids[i].getX(), asteroids[i].getY(), px, py))
		{
			player->die();
			asteroids.erase(asteroids.begin() + i);
			px = player->getX();
			py = player->getY();
		}
		else
			i++;
	}

	// Nemici vs giocatore (nemico largo 3: ex-1..ex+1)
	for (size_t i = 0; i < enemies.size();)
	{
		int ex = enemies[i].getX();
		int ey = enemies[i].getY();

		bool collision = hitsPlayer(ex - 1, ey, px, py) ||
						 hitsPlayer(ex, ey, px, py) ||
						 hitsPlayer(ex + 1, ey, px, py);
		if (collision)
		{
			player->die();
			enemies.erase(enemies.begin() + i);
			px = player->getX();
			py = player->getY();
		}
		else
			i++;
	}
}
