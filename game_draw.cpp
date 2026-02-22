#include "includes/game.hpp"
#include "includes/utilities.hpp"

void Game::updateBackground()
{
	const char starChars[] = {'.', '+', '*', '`'};
	for (auto &s : stars)
	{
		// Le stelle dim si muovono lentamente, le bright più veloci
		if (s.color == COLOR_STAR_BRIGHT && frameCount % 5 == 0)
			s.y++;
		else if (s.color == COLOR_STAR_DIM && frameCount % 10 == 0)
			s.y++;

		if (s.y >= win_height - 1)
		{
			s.y = 1;
			s.x = RandomBetweenInt(1, win_width - 2);
			s.ch = starChars[RandomBetweenInt(0, 3)];
			s.color = (RandomBetweenInt(0, 2) == 0) ? COLOR_STAR_BRIGHT : COLOR_STAR_DIM;
		}
	}
}

void Game::drawBackground()
{
	for (const auto &s : stars)
	{
		if (s.y > 0 && s.y < win_height - 1 && s.x > 0 && s.x < win_width - 1)
		{
			wattron(win, COLOR_PAIR(s.color));
			if (s.color == COLOR_STAR_DIM)
				wattron(win, A_DIM);
			mvwaddch(win, s.y, s.x, s.ch);
			if (s.color == COLOR_STAR_DIM)
				wattroff(win, A_DIM);
			wattroff(win, COLOR_PAIR(s.color));
		}
	}
}

void Game::drawLives()
{
	wattron(win, COLOR_PAIR(COLOR_HUD) | A_BOLD);
	mvwprintw(win, 0, win_width - 20, "Lives: %d", player->getHealth());
	wattroff(win, COLOR_PAIR(COLOR_HUD) | A_BOLD);
}

void Game::drawHealthBar()
{
	int barWidth = 30;
	int playerHealth = player->getHealth();
	int maxPlayerHealth = 100;
	int filledWidth = (playerHealth * barWidth) / maxPlayerHealth;

	wattron(win, COLOR_PAIR(COLOR_HUD));
	mvwaddstr(win, WIN_HEIGHT - 1, 2, "HP[");
	wattroff(win, COLOR_PAIR(COLOR_HUD));

	int hpColor = (playerHealth > 30) ? COLOR_HP_FULL : COLOR_HP_LOW;
	wattron(win, COLOR_PAIR(hpColor) | A_BOLD);
	for (int i = 0; i < filledWidth; i++)
	{
		mvwaddch(win, WIN_HEIGHT - 1, 5 + i, '=');
	}
	wattroff(win, COLOR_PAIR(hpColor) | A_BOLD);

	wattron(win, COLOR_PAIR(COLOR_HUD));
	mvwaddstr(win, WIN_HEIGHT - 1, 5 + barWidth, "]");
	mvwprintw(win, WIN_HEIGHT - 1, 5 + barWidth + 3, "%d/%d", playerHealth, maxPlayerHealth);
	wattroff(win, COLOR_PAIR(COLOR_HUD));

}

void Game::drawHUD()
{
	wattron(win, COLOR_PAIR(COLOR_HUD) | A_BOLD);
	mvwprintw(win, 0, 2, "Score: %d", score);

	// Clock-based: tempo di gioco (accumulato via delta time)
	mvwprintw(win, 0, win_width / 2 - 5, "Time: %ds", (int)elapsedTime);

	// Clock-based: FPS reali
	mvwprintw(win, 0, win_width - 12, "FPS: %.0f", currentFPS);
	wattroff(win, COLOR_PAIR(COLOR_HUD) | A_BOLD);

	drawHealthBar();
	drawLives();
}

void Game::drawExplosion()
{
	if (!player->getIsDead())
		return;

	int px = player->getX();
	int py = player->getY();

	if (py < 1 || py >= win_height - 1)
		return;

	wattron(win, COLOR_PAIR(COLOR_EXPLOSION) | A_BOLD);
	mvwaddch(win, py - 1, px - 1, '*');
	mvwaddch(win, py - 1, px, '*');
	mvwaddch(win, py - 1, px + 1, '*');

	mvwaddch(win, py, px - 1, '*');
	mvwaddch(win, py, px, 'X');
	mvwaddch(win, py, px + 1, '*');

	mvwaddch(win, py + 1, px - 1, '*');
	mvwaddch(win, py + 1, px, '*');
	mvwaddch(win, py + 1, px + 1, '*');
	wattroff(win, COLOR_PAIR(COLOR_EXPLOSION) | A_BOLD);
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

	// Bordo colorato
	wattron(win, COLOR_PAIR(COLOR_HUD));
	box(win, 0, 0);
	wattroff(win, COLOR_PAIR(COLOR_HUD));

	// Sfondo stellare
	drawBackground();

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

		wattron(win, COLOR_PAIR(COLOR_GAMEOVER) | A_BOLD);
		mvwaddstr(win, win_height / 2 - 2, (win_width - 10) / 2, "GAME OVER");
		wattroff(win, COLOR_PAIR(COLOR_GAMEOVER) | A_BOLD);

		wattron(win, COLOR_PAIR(COLOR_HUD));
		mvwprintw(win, win_height / 2, (win_width - 10) / 2, "Final score: %d", score);
		mvwaddstr(win, win_height / 2 + 2, (win_width - 10) / 2, "Press R to restart");
		wattroff(win, COLOR_PAIR(COLOR_HUD));
	}

	wrefresh(win);
}

void Game::drawPlayer()
{
	wattron(win, COLOR_PAIR(COLOR_PLAYER) | A_BOLD);
	mvwaddstr(win, player->getY(), player->getX(), "A");
	mvwaddstr(win, player->getY() + 1, player->getX() - 1, "/|\\");
	wattroff(win, COLOR_PAIR(COLOR_PLAYER) | A_BOLD);
}

void Game::drawBullets()
{
	wattron(win, COLOR_PAIR(COLOR_PLAYER_BULLET) | A_BOLD);
	for (const auto &b : p_bullets)
	{
		mvwaddch(win, b.getY(), b.getX(), '|');
	}
	wattroff(win, COLOR_PAIR(COLOR_PLAYER_BULLET) | A_BOLD);

	wattron(win, COLOR_PAIR(COLOR_ENEMY_BULLET) | A_BOLD);
	for (const auto &b : e_bullets)
	{
		mvwaddch(win, b.getY(), b.getX(), '*');
	}
	wattroff(win, COLOR_PAIR(COLOR_ENEMY_BULLET) | A_BOLD);
}

void Game::drawEnemies()
{
	for (const auto &e : enemies)
	{
		int color;
		if (e.getHealth() <= 1)
			color = COLOR_ENEMY_DEAD;
		else if (e.getHealth() <= 3)
			color = COLOR_ENEMY_DAMAGED;
		else
			color = COLOR_ENEMY_FULL;
		wattron(win, COLOR_PAIR(color));
		mvwaddch(win, e.getY(), e.getX() - 1, '(');
		mvwaddch(win, e.getY(), e.getX(), 'O');
		mvwaddch(win, e.getY(), e.getX() + 1, ')');
		wattroff(win, COLOR_PAIR(color));
	}
}

void Game::drawAsteroids()
{
	wattron(win, COLOR_PAIR(COLOR_ASTEROID) | A_BOLD);
	for (const auto &a : asteroids)
	{
		mvwaddstr(win, a.getY(), a.getX(), "@");
	}
	wattroff(win, COLOR_PAIR(COLOR_ASTEROID) | A_BOLD);
}
