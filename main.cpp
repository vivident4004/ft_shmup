#include "includes/game.hpp"
#include <chrono>
#include <unistd.h>

static void start_ncurses(void)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
    curs_set(0);

	// Inizializza colori
	if (has_colors()) {
		start_color();
		use_default_colors();
		if (can_change_color()) {
			init_color(COLOR_BROWN, 600, 400, 200);
			init_color(COLOR_ENEMY_FULL, 964, 188, 286);
			init_color(COLOR_ENEMY_DAMAGED, 816, 153, 326);
			init_color(COLOR_ENEMY_DEAD, 538, 141, 294);
		}

		init_pair(COLOR_PLAYER, COLOR_CYAN, -1);
		init_pair(COLOR_PLAYER_BULLET, COLOR_GREEN, -1);
		init_pair(COLOR_ENEMY_FULL, COLOR_ENEMY_FULL, -1);
		init_pair(COLOR_ENEMY_DAMAGED, COLOR_ENEMY_DAMAGED, -1);
		init_pair(COLOR_ENEMY_DEAD, COLOR_ENEMY_DEAD, -1);
		init_pair(COLOR_ENEMY_BULLET, COLOR_RED, -1);
		init_pair(COLOR_ASTEROID, COLOR_BROWN, -1);
		init_pair(COLOR_HUD, COLOR_WHITE, -1);
		init_pair(COLOR_HP_FULL, COLOR_GREEN, -1);
		init_pair(COLOR_HP_LOW, COLOR_RED, -1);
		init_pair(COLOR_STAR_DIM, COLOR_BLUE, -1);
		init_pair(COLOR_STAR_BRIGHT, COLOR_WHITE, -1);
		init_pair(COLOR_EXPLOSION, COLOR_YELLOW, -1);
		init_pair(COLOR_GAMEOVER, COLOR_RED, -1);
	}
}

int main()
{
	start_ncurses();

	Game game;
	int c = 0;

	// Clock-based: delta time con std::chrono
	using Clock = std::chrono::steady_clock;
	using Duration = std::chrono::duration<double>;

	const double frameDurationSec = 1.0 / FPS; // ~0.01667s per 60 FPS
	auto lastTime = Clock::now();

	while (c != 'q' && c != 'Q')
	{
		// Delta time: tempo reale trascorso dall'ultimo frame
		auto now = Clock::now();
		double deltaTime = Duration(now - lastTime).count();
		lastTime = now;

		// Clock-based: accumula tempo di gioco
		game.elapsedTime += deltaTime;

		// Clock-based: calcola FPS reali
		game.fpsAccumulator += deltaTime;
		game.fpsFrameCount++;
		if (game.fpsAccumulator >= 1.0) {
			game.currentFPS = game.fpsFrameCount / game.fpsAccumulator;
			game.fpsFrameCount = 0;
			game.fpsAccumulator = 0.0;
		}

		// Input
		while ((c = getch()) != ERR && c != 'q' && c != 'Q')
			game.handleInput(c);

		// Pausa: aspetta che il giocatore riprenda
		if (game.paused) {
			nodelay(stdscr, FALSE);
			while (game.paused) {
				c = getch();
				if (c == KEY_RESIZE)
					game.render();
				if (c == 'p')
					game.paused = false;
				if (c == 'q' || c == 'Q')
					break;
			}
			nodelay(stdscr, TRUE);
			if (c == 'q' || c == 'Q')
				break;
			lastTime = Clock::now(); // Reset delta dopo pausa
			continue;
		}

		// Frame-based: 1 update + 1 render per frame
		game.update();
		game.render();

		// Throttling: dormi il tempo restante del frame
		auto frameEnd = Clock::now();
		double frameElapsed = Duration(frameEnd - now).count();
		double sleepSec = frameDurationSec - frameElapsed;
		if (sleepSec > 0.0) {
			usleep((useconds_t)(sleepSec * 1000000));
		}
	}

	endwin();
	return (0);
}
