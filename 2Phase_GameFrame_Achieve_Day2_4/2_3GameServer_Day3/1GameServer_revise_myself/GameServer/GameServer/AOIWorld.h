#pragma once
// #include "GameRole.h"
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
	// static int num;

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

	std::vector<Grid> m_grids;

	std::list<Player*> GetSrdPlayersPosition(Player* _player);

	bool Add_Player(Player* _player);
	void Del_Player(Player* _player);
};

