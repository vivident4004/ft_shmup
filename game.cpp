#include "game.hpp"
#include "player.hpp"

void Game::updatePlayer(Player& player)
{
	int player_x = player.getX();
	int player_y = player.getY();

	int c = getch();
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
		player.shoot();
	}
}

void Game::updateBullets(vector<Bullet> &bullets)
{
	for (auto& bullet : bullets)
	{
		int newY = bullet.getY() + bullet.direction;
		bullet.setY(newY);
	}
}

void Game::drawPlayer(Player &player)
{
	mvwaddstr(this->win, player.getY(), player.getX(), "A");
	mvwaddstr(this->win, player.getY() + 1, player.getX() - 1, "/|\\");
}

// void Game::drawBullets(vector<Bullet> &bullets) 
// {
// 	for (const auto& bullet : bullets)
// 	{
// 		(wmove((win),(bullet.getY()),( bullet.getX())) == ERR ? ERR : waddnstr((win),( "|"),-1));
// 	}
// }