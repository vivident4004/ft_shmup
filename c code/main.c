#include "header.h"
// clear; make && ./ft_shmup
// var globali
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

int main(int ac, char **av)
{
	(void) ac;
	(void) av;
	start_ncurses();
	
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	
	// Dimensioni finestra di gioco
	// TODO da mettere in header.h
	int win_width = 211;
	int win_height = 54;
	
	// Controlla se il terminale è abbastanza grande
	// if (max_x < win_width || max_y < win_height)
	// {
	// 	//endwin();
	// 	printf("Terminale troppo piccolo! Richiesto almeno %dx%d\n", win_width, win_height);
	// 	return (1);
	// }
	
	// Crea una finestra
	WINDOW *game_win = newwin(win_height, win_width, 0, 0);
	box(game_win, 0, 0); // Disegna bordi
	
	int ship_x = win_width / 2 - 1;
	int ship_y = win_height - 3;
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
		if (max_x < win_width || max_y < win_height)
		{
			game_paused = 1;
			clear();
			mvprintw(max_y / 2, (max_x - 40) / 2, "Terminale troppo piccolo!");
			mvprintw(max_y / 2 + 1, (max_x - 40) / 2, "Richiesto almeno %dx%d", win_width, win_height);
			mvprintw(max_y / 2 + 2, (max_x - 40) / 2, "Ridimensiona per continuare...");
			refresh();
			c = getch();
			// TODO da cambiare
			usleep(100000); // Sleep 100ms per non loopare troppo velocemente
			continue;
		}
		
		// Se il gioco era in pausa e le dimensioni sono di nuovo giuste, riprendi
		if (game_paused)
		{
			game_paused = 0;
			clear();
			refresh();
			delwin(game_win);
			game_win = newwin(win_height, win_width, 0, 0);
			old_max_x = max_x;
			old_max_y = max_y;
		}
		
		werase(game_win);
		box(game_win, 0, 0);
		
		// Leggi input
		c = getch();
		if (c == KEY_LEFT && ship_x > 2) ship_x--;
		else if (c == KEY_RIGHT && ship_x < win_width - 3) ship_x++;
		else if (c == KEY_UP && ship_y > 2) ship_y--;
		else if (c == KEY_DOWN && ship_y < win_height - 3) ship_y++;
		mvwaddstr(game_win, ship_y, ship_x, "#");
		mvwaddstr(game_win, ship_y + 1, ship_x - 1, "@@@");
		
		wrefresh(game_win);
	} while (c != 27); // ESC
	
	delwin(game_win);
	endwin();
	return (0);
}