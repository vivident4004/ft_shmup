# include <unistd.h>
# include <curses.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>

#define MIN_WIN_X 90
#define MIN_WIN_Y 45

typedef struct entity
{
	int type; // 0 = player, 1 = nemico, 2 = proiettile
	int y;
	int x;
	int hp;
	int active; // 1 se l'entità è attiva, 0 se è da rimuovere
	struct entity *next;
} t_entity;

typedef struct s_game {
    WINDOW *win;
    t_entity *entities;
    int score;
	int win_width;
	int win_height;
} t_game;

typedef struct position {
    int x;
    int y;
} pos;



extern pos WIN_SIZE;


void draw_grid();