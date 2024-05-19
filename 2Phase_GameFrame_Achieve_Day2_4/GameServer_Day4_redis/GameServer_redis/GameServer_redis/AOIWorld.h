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
	std::vector<Grid> m_grids;

	int x_begin;
	int x_end;
	int y_begin;
	int y_end;
	int x_count;
	int y_count;

	int x_width;
	int y_width;
public:

	AOIWorld(int _x_begin, int _x_end, int _y_begin, int _y_end, int _x_count, int _y_count);

	std::list<Player*> GetSrdPlayerPosition(Player* _player);
	bool Add_Player(Player* _player);
	bool Del_Player(Player* _player);
};

