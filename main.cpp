#include "includes/game.hpp"
#include <ctime>
// ctime.h
// (long )scr(getCurrentTime) / CLOSCK_PER_SEC
// framecount % (CLOCKS_PER_SEC / 60) == 0
static void start_ncurses(void)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE); // Abilita i tasti freccia
	nodelay(stdscr, TRUE); // Imposta getch() non bloccante
    curs_set(0); // Nasconde il cursore
}

int main()
{
	start_ncurses();
	
	Game game;
	int c = 0;

	unsigned long int frameCount = 0;

	while (c != 27) // ESC per uscire
	{
		c = getch();

		game.handleInput(c);
		if (frameCount % (CLOCKS_PER_SEC / 60) == 0) {
			game.update();
			game.render();
		}
		
		// usleep(10000);
		frameCount++;
	}
	
	endwin();
	return (0);
}
