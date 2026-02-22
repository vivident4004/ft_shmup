#include "includes/game.hpp"
#include "includes/utilities.hpp"

Game::Game() : win(nullptr),
			   keyLeft(false), keyRight(false), keyUp(false),
			   keyDown(false), keyShooting(false),
			   score(0), currentFPS(0.0), elapsedTime(0.0),
			   fpsAccumulator(0.0), fpsFrameCount(0),
			   frameCount(0), lastSpawnFrame(0), lastEnemyMoveFrame(0)
{
	this->win_width = WIN_WIDTH;
	this->win_height = WIN_HEIGHT;
	this->player = new Player(WIN_WIDTH / 2, WIN_HEIGHT - 3, this, 100);
	this->paused = false;
}

Game::~Game()
{
	if (this->win)
	{
		delwin(this->win);
	}
	delete this->player;
}

void Game::addBullet(const Bullet &bullet, int entity)
{
	if (entity == PLAYER)
		p_bullets.push_back(bullet);
	else if (entity == ENEMY)
		e_bullets.push_back(bullet);
}

void Game::addAsteroid(const Asteroid &asteroid)
{
	asteroids.push_back(asteroid);
}

void Game::spawnEnemy()
{
	int x = RandomBetweenInt(1, win_width - 2);
	enemies.emplace_back(x, 1, this, 5);
}

void Game::handleInput(int c)
{
	if (player->getIsDead())
	{
		if (c == 'r' || c == 'R')
		{
			player->revive();
			score = 0;
			frameCount = 0;
			lastSpawnFrame = 0;
			lastEnemyMoveFrame = 0;
			elapsedTime = 0.0;
			fpsAccumulator = 0.0;
			fpsFrameCount = 0;
			enemies.clear();
			p_bullets.clear();
			e_bullets.clear();
			asteroids.clear();
		}
		return;
	}

	// Input normale del gioco
	switch (c)
	{
	case KEY_LEFT:
		if (player->getX() > 2)
			player->setX(player->getX() - 1);
		break;
	case KEY_RIGHT:
		if (player->getX() < win_width - 3)
			player->setX(player->getX() + 1);
		break;
	case KEY_UP:
		if (player->getY() > 1)
			player->setY(player->getY() - 1);
		break;
	case KEY_DOWN:
		if (player->getY() < win_height - 3)
			player->setY(player->getY() + 1);
		break;
	case ' ':
		player->shoot();
		break;
	case 'p':
		paused = !paused;
		break;
	}
}

void Game::update()
{
	frameCount++;
	if (frameCount - lastSpawnFrame >= SPAWN_INTERVAL_FRAMES)
	{
		spawnEnemy();
		addAsteroid(Asteroid(RandomBetweenInt(1, win_width - 2), 1));
		lastSpawnFrame = frameCount;
	}

	// updateBullet() è chiamato ogni frame
	updateBullets();
	checkCollisions();

	if (frameCount - lastEnemyMoveFrame >= ENEMY_MOVE_INTERVAL_FRAMES)
	{
		updateEnemies();
		updateAsteroids();
		lastEnemyMoveFrame = frameCount;
	}
}

void Game::updateBullets()
{
	// Proiettili del giocatore
	for (size_t i = 0; i < p_bullets.size();)
	{
		// p_bullets[i].setY(p_bullets[i].getY() + p_bullets[i].direction);
		Bullet &b = p_bullets[i];
		b.tick++;
		if (b.tick >= b.speed)
		{
			b.setY(b.getY() + b.direction);
			b.tick = 0;
		}
		// se il proiettile esce dallo schermo, rimuovilo
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
		// e_bullets[i].setY(e_bullets[i].getY() + e_bullets[i].direction);
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

void Game::drawHealthBar()
{
	int barWidth = 30;
	int playerHealth = player->getHealth();
	int maxPlayerHealth = 100;
	int filledWidth = (playerHealth * barWidth) / maxPlayerHealth;

	mvwaddstr(win, WIN_HEIGHT - 1, 2, "HP[");

	for (int i = 0; i < filledWidth; i++)
	{
		mvwaddch(win, WIN_HEIGHT - 1, 5 + i, '=');
	}

	mvwaddstr(win, WIN_HEIGHT - 1, 5 + barWidth, "]");
	mvwprintw(win, WIN_HEIGHT - 1, 5 + barWidth + 3, "%d/%d", playerHealth, maxPlayerHealth);
}

void Game::drawHUD()
{
	mvwprintw(win, 0, 2, "Score: %d", score);

	// Clock-based: tempo di gioco (accumulato via delta time)
	mvwprintw(win, 0, win_width / 2 - 5, "Time: %ds", (int)elapsedTime);

	// Clock-based: FPS reali
	mvwprintw(win, 0, win_width - 12, "FPS: %.0f", currentFPS);

	drawHealthBar();
}

void Game::drawExplosion()
{
	if (!player->getIsDead())
		return;

	int px = player->getX();
	int py = player->getY();

	if (py < 1 || py >= win_height - 1)
		return;

	mvwaddch(win, py - 1, px - 1, '*');
	mvwaddch(win, py - 1, px, '*');
	mvwaddch(win, py - 1, px + 1, '*');

	mvwaddch(win, py, px - 1, '*');
	mvwaddch(win, py, px, 'X');
	mvwaddch(win, py, px + 1, '*');

	mvwaddch(win, py + 1, px - 1, '*');
	mvwaddch(win, py + 1, px, '*');
	mvwaddch(win, py + 1, px + 1, '*');
}

void Game::render()
{
	// Legge dimensione del terminale ad ogni render
	int term_h, term_w;
	getmaxyx(stdscr, term_h, term_w);

	// Se il terminale è troppo piccolo rispetto ai requisiti minimi, mostra pausa e ritorna
	if (term_w < WIN_WIDTH || term_h < WIN_HEIGHT)
	{
		paused = true;
		if (win)
		{
			delwin(win);
			win = nullptr;
		}
		clear();
		int col = (term_w - 40) / 2;
		if (col < 0)
			col = 0;
		mvprintw(term_h / 2, col, "Terminale troppo piccolo!");
		mvprintw(term_h / 2 + 1, col, "Richiesto almeno %dx%d", WIN_WIDTH, WIN_HEIGHT);
		mvprintw(term_h / 2 + 2, col, "Ridimensiona per continuare...");
		refresh();
		return;
	}

	// Limita la dimensione di gioco al massimo definito in includes/game.hpp
	int new_h = (term_h > WIN_HEIGHT) ? WIN_HEIGHT : term_h;
	int new_w = (term_w > WIN_WIDTH) ? WIN_WIDTH : term_w;

	win_height = new_h;
	win_width = new_w;

	// Assicurati che la WINDOW esista e corrisponda alle dimensioni desiderate
	if (!win)
	{
		win = newwin(win_height, win_width, 0, 0);
	}
	else
	{
		int h, w;
		getmaxyx(win, h, w);
		if (h != win_height || w != win_width)
		{
			delwin(win);
			win = newwin(win_height, win_width, 0, 0);
		}
	}
	paused = false;
	werase(win);
	box(win, 0, 0);

	drawHUD();

	if (!player->getIsDead())
	{
		drawPlayer();
		drawBullets();
		drawEnemies();
		drawAsteroids();
		drawHealthBar();
	}
	else
	{
		drawExplosion();
		mvwaddstr(win, win_height / 2 - 2, (win_width - 10) / 2, "GAME OVER");
		mvwprintw(win, win_height / 2, (win_width - 10) / 2, "Final score: %d", score);
		mvwaddstr(win, win_height / 2 + 2, (win_width - 10) / 2, "Press R to restart");
	}

	wrefresh(win);
}

void Game::drawPlayer()
{
	mvwaddstr(win, player->getY(), player->getX(), "A");
	mvwaddstr(win, player->getY() + 1, player->getX() - 1, "/|\\");
}

void Game::drawBullets()
{
	for (const auto &b : p_bullets)
	{
		mvwaddch(win, b.getY(), b.getX(), '|');
	}
	for (const auto &b : e_bullets)
	{
		mvwaddch(win, b.getY(), b.getX(), '*');
	}
}

void Game::drawEnemies()
{
	for (const auto &e : enemies)
	{
		mvwaddch(win, e.getY(), e.getX() - 1, '(');
		mvwaddch(win, e.getY(), e.getX(), 'O');
		mvwaddch(win, e.getY(), e.getX() + 1, ')');
	}
}

void Game::drawAsteroids()
{
	for (const auto &a : asteroids)
	{
		mvwaddstr(win, a.getY(), a.getX(), "@");
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

		// Il giocatore occupa: (px, py) e (px, py+1)
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
