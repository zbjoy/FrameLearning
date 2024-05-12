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
	std::list<Player* > m_player_list;
};

class AOIWorld
{
	int x_begin = 0;
	int x_end = 0;
	int y_begin = 0;
	int y_end = 0;
	int x_count = 0;
	int y_count = 0;

	int x_width = 0;
	int y_width = 0;

public:
	std::vector<Grid> m_grids;

	AOIWorld(int _x_begin, int _x_end, int _y_begin, int _y_end, int _x_count, int _y_count);

	std::list<Player*> GetSrdPlayer(Player* _player);

	bool Add_Player(Player* _player);
	bool Del_Player(Player* _player);

};

