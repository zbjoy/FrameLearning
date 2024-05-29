#pragma once
#include <vector>
#include <list>

class Player
{
public:
	virtual float GetX() = 0;
	virtual float GetY() = 0;
};

class Grid
{
public:
	std::list<Player*> m_player_list;
};

class AOIWorld
{
public:
	AOIWorld(int _x_begin, int _x_end, int _y_begin, int _y_end, int _x_count, int _y_count);

	/* 获取玩家周围位置 */
	std::list<Player*> GetSrdPlayersPosition(Player* _player);

	/* 添加玩家 */
	bool Add_Player(Player* _player);

	/* 删除玩家 */
	void Del_Player(Player* _player);

private:
	std::vector<Grid> m_grid_list;

	int x_begin;
	int x_end;
	int y_begin;
	int y_end;

	int x_count;
	int y_count;

	int x_width;
	int y_width;
};

