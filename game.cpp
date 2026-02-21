#include "game.hpp"
#include "player.hpp"

void Game::updatePlayer(Player& player, vector<Bullet> &bullets)
{
	int player_x = player.getPosX();
	int player_y = player.getPosY();

	char c = getch();
	if (c == KEY_LEFT && player_x > 2)
		player_x--;
	else if (c == KEY_RIGHT && player_x < W_W - 3)
		player_x++;
	else if (c == KEY_UP && player_y > 2)
		player_y--;
	else if (c == KEY_DOWN && player_y < W_H - 3)
		player_y++;
	else if (c == ' ')
	{
		Player::shoot(-1);
	}
}