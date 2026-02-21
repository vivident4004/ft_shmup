#include "header.h"
// tutto ciò che riguarda ncurses
// int draw_frame(); // pulisce e ridisegna basandosi sulla lista delle entità

// pos WIN_SIZE;


// static void set_win_size(void) {
//     // Tamanho da tela
//     getmaxyx(stdscr, WIN_SIZE.y, WIN_SIZE.x);

//     // Verifica se a WIN_SIZE nao esta menor que o tamanho minimo
//     if (WIN_SIZE.x < MIN_WIN_X) {
//         WIN_SIZE.x = MIN_WIN_X;
//     }
//     if (WIN_SIZE.y < MIN_WIN_Y) {
//         WIN_SIZE.y = MIN_WIN_Y;
//     }
// }

// void draw_grid()
// {
//     // int max_y, max_x;
//     // getmaxyx(stdscr, WIN_SIZE.y, WIN_SIZE.y);
// 	set_win_size();
//     const char *legend = "</^/>/v: muovi  |  q: torna al menú";
//     int start_x = (WIN_SIZE.x - (int)strlen(legend)) / 2;
//     mvprintw(WIN_SIZE.y - 2, start_x, "%s", legend);
//     mvhline(WIN_SIZE.y - 1, 0, ACS_HLINE, WIN_SIZE.x);
//     refresh();
// }

