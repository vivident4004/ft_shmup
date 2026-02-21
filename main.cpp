#include "game.hpp"
#include "player.hpp"

int SCORE = 0;

static void start_ncurses(void)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE); // Abilita i tasti freccia
	nodelay(stdscr, TRUE); // Imposta getch() non bloccante
    curs_set(0); // Nasconde il cursore
	// start_color();
}

int main()
{
	Game game;
	start_ncurses();
	
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	
	// Crea una finestra
	// WINDOW *game_win = newwin(W_H, W_W, 0, 0);
	box(game.win, 0, 0); // Disegna bordi
	
	Player player(W_W / 2 - 1, W_H - 3);
	// int ship_x = W_W / 2 - 1;
	// int ship_y = W_H - 3;
	
	int c = 0;
	
	int old_max_x = max_x;
	int old_max_y = max_y;
	int game_paused = 0;

	// TODO: da spostare logica in game.c
	do
	{
		// Controlla se il terminale è stato ridimensionato
		getmaxyx(stdscr, max_y, max_x);
		
		// Se il terminale è troppo piccolo, blocca il gioco
		// TODO: da spostare in una funzione a parte e chiamarla ogni volta che si ridimensiona
		// if (max_x < W_W || max_y < W_H)
		// {
		// 	game_paused = 1;
		// 	clear();
		// 	mvprintw(max_y / 2, (max_x - 40) / 2, "Terminale troppo piccolo!");
		// 	mvprintw(max_y / 2 + 1, (max_x - 40) / 2, "Richiesto almeno %dx%d", W_W, W_H);
		// 	mvprintw(max_y / 2 + 2, (max_x - 40) / 2, "Ridimensiona per continuare...");
		// 	refresh();
		// 	c = getch();
		// 	// TODO da cambiare
		// 	usleep(100000); // Sleep 100ms per non loopare troppo velocemente
		// 	continue;
		// }
		
		// Se il gioco era in pausa e le dimensioni sono di nuovo giuste, riprendi
		if (game_paused)
		{
			game_paused = 0;
			clear();
			refresh();
			delwin(game.win);
			game.win = newwin(W_H, W_W, 0, 0);
			old_max_x = max_x;
			old_max_y = max_y;
		}
		
		werase(game.win);
		box(game.win, 0, 0);
		
		// Leggi input
		game.updatePlayer(player);
		game.drawPlayer(player);
		game.updateBullets(player.getPlayerBullets());
		
		wrefresh(game.win);
	} while (c != 27); // ESC
	
	game.~Game();
	endwin();
	return (0);
}