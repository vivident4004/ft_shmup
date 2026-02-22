#include "includes/game.hpp"
#include "includes/utilities.hpp"
#include <ctime>

Game::Game() : win(nullptr), score(0) {
	this->win_width = WIN_WIDTH;
	this->win_height = WIN_HEIGHT;
	this->player = new Player(WIN_WIDTH / 2, WIN_HEIGHT - 3, this);
	this->paused = false;
	lastSpawnClock = clock();
	spawnIntervalTicks = CLOCKS_PER_SEC * 1;
	lastEnemyMoveClock = clock();
	enemyMoveIntervalTicks = CLOCKS_PER_SEC * 1;
}

Game::~Game() {
	if (this->win) {
		delwin(this->win);
	}
	delete this->player;
}

void Game::addBullet(const Bullet& bullet, int entity) {
	if (entity == PLAYER)
		p_bullets.push_back(bullet);
	else if (entity == ENEMY)
		e_bullets.push_back(bullet);
}

void Game::handleInput(int c) {
	switch(c) {
		case KEY_LEFT:
			if (player->getX() > 2) player->setX(player->getX() - 1);
			break;
		case KEY_RIGHT:
			if (player->getX() < win_width - 3) player->setX(player->getX() + 1);
			break;
		case KEY_UP:
			if (player->getY() > 1) player->setY(player->getY() - 1);
			break;
		case KEY_DOWN:
			if (player->getY() < win_height - 3) player->setY(player->getY() + 1);
			break;
		case ' ':
			player->shoot();
			break;
		case 'p':
			paused = !paused;
			break;
	}
}

void Game::update() {
	clock_t now = clock();
	if (now - lastSpawnClock >= spawnIntervalTicks) {
		spawnEnemy();
		lastSpawnClock = now;
	}

	// updateBullet() è chiamato ogni frame, ma i proiettili si muovono solo quando scade il timer
	updateBullets();

	// muove i nemici solo quando scade il timer
	if (now - lastEnemyMoveClock >= enemyMoveIntervalTicks) {
		updateEnemies();
		lastEnemyMoveClock = now;
	}

	// TODO: collisioni (proiettili vs nemici, proiettili vs giocatore)
	// TODO: aggiungere e rimuovere proiettili nemici
	// TODO: asteroidi che scorrono
	// TODO: game over, punteggio, ecc. (da implementare dall'utente)
}

void Game::updateBullets() {
	// Proiettili del giocatore
	for (size_t i = 0; i < p_bullets.size(); )
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
	for (size_t i = 0; i < e_bullets.size(); )
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

void Game::spawnEnemy() {
	int x = RandomBetweenInt(1, win_width - 2);
	enemies.emplace_back(x, 1, this, 1);
}

void Game::updateEnemies()
{
    for (size_t i = 0; i < enemies.size(); )
    {
        Enemy &e = enemies[i];
        int newX = e.getX() + RandomBetweenInt(-3, 3);
        
        // ✔ Mantieni i nemici dentro lo schermo
        if (newX < 2) newX = 2;
        if (newX > win_width - 3) newX = win_width - 3;
        
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

void Game::render() {
	// Legge dimensione del terminale ad ogni render
	int term_h, term_w;
	getmaxyx(stdscr, term_h, term_w);

	// Se il terminale è troppo piccolo rispetto ai requisiti minimi, mostra pausa e ritorna
	if (term_w < WIN_WIDTH || term_h < WIN_HEIGHT)
	{
		paused = true;
		if (win) {
			delwin(win);
			win = nullptr;
		}
		clear();
		int col = (term_w - 40) / 2;
		if (col < 0) col = 0;
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
	if (!win) {
		win = newwin(win_height, win_width, 0, 0);
	} else {
		int h, w;
		getmaxyx(win, h, w);
		if (h != win_height || w != win_width) {
			delwin(win);
			win = newwin(win_height, win_width, 0, 0);
		}
	}
	paused = false;
	werase(win);
	box(win, 0, 0);

	drawPlayer();
	drawBullets();
	drawEnemies();

	wrefresh(win);
}

void Game::drawPlayer() {
	mvwaddstr(win, player->getY(), player->getX(), "A");
	mvwaddstr(win, player->getY() + 1, player->getX() - 1, "/|\\");
}

void Game::drawBullets() {
	for (const auto& b : p_bullets) {
		mvwaddch(win, b.getY(), b.getX(), '|');
	}
	for (const auto& b : e_bullets) {
		mvwaddch(win, b.getY(), b.getX(), '*');
	}
}

void Game::drawEnemies() {
	for (const auto& e : enemies) {
		mvwaddch(win, e.getY(), e.getX() - 1, '(');
		mvwaddch(win, e.getY(), e.getX(), 'O');
		mvwaddch(win, e.getY(), e.getX() + 1, ')');
	}
}

void Game::checkCollisions() {
	
}
