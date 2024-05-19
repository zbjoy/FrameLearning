#include "AOIWorld.h"
#include <iostream>
#include <algorithm>

AOIWorld::AOIWorld(int _x_begin, int _x_end, int _y_begin, int _y_end, int _x_count, int _y_count) : x_begin(_x_begin), x_end(_x_end), y_begin(_y_begin), y_end(_y_end), x_count(_x_count), y_count(_y_count)
{
	x_width = (x_end - x_begin) / x_count;
	y_width = (y_end - y_begin) / y_count;

	for (int i = 0; i < x_count * y_count; i++)
	{
		Grid temp;
		m_grids.push_back(temp);
	}
}

std::list<Player*> AOIWorld::GetSrdPlayerPosition(Player* _player)
{
	std::list<Player*> retList;
	/* --------------------------------------------------------- */
	//int x_index = ((int)_player->GetX() - x_begin) / x_width;
	//int y_index = ((int)_player->GetY() - y_begin) / y_width;

	//int grid = x_index + y_index * x_count;
	int grid = ((int)_player->GetX() - x_begin) / x_width + ((int)_player->GetY() - y_begin) / y_width * x_count;

	int x_index = grid % x_count;
	int y_index = grid / x_count;
	/* --------------------------------------------------------- */


	if (x_index > 0 && y_index > 0)
	{
		std::list<Player*> temp = m_grids[grid - 1 - x_count].m_player_list;
		retList.insert(retList.end(), temp.begin(), temp.end());
	}
	if (y_index > 0)
	{
		std::list<Player*> temp = m_grids[grid - x_count].m_player_list;
		retList.insert(retList.end(), temp.begin(), temp.end());
	}
	if (x_index < x_count - 1 && y_index > 0)
	{
		std::list<Player*> temp = m_grids[grid + 1 - x_count].m_player_list;
		retList.insert(retList.end(), temp.begin(), temp.end());
	}
	if (x_index > 0)
	{
		std::list<Player*> temp = m_grids[grid - 1].m_player_list;
		retList.insert(retList.end(), temp.begin(), temp.end());
	}

	std::list<Player*> temp = m_grids[grid].m_player_list;
	retList.insert(retList.end(), temp.begin(), temp.end());

	if (x_index < x_count - 1)
	{
		//std::list<Player*> temp = m_grids[grid - 1].m_player_list;
		std::list<Player*> temp = m_grids[grid + 1].m_player_list;
		retList.insert(retList.end(), temp.begin(), temp.end());
	}
	if (x_index > 0 && y_index < y_count - 1)
	{
		// std::list<Player*> temp = m_grids[grid - 1].m_player_list;
		std::list<Player*> temp = m_grids[grid - 1 + x_count].m_player_list;
		retList.insert(retList.end(), temp.begin(), temp.end());
	}
	if (y_index < y_count - 1)
	{
		// std::list<Player*> temp = m_grids[grid - 1].m_player_list;
		std::list<Player*> temp = m_grids[grid + x_count].m_player_list;
		retList.insert(retList.end(), temp.begin(), temp.end());
	}
	if (x_index < x_count - 1 && y_index > y_count - 1)
	{
		// std::list<Player*> temp = m_grids[grid - 1].m_player_list;
		std::list<Player*> temp = m_grids[grid + 1 + x_count].m_player_list;
		retList.insert(retList.end(), temp.begin(), temp.end());
	}

	return retList;
}

bool AOIWorld::Add_Player(Player* _player)
{
	int grid = ((int)_player->GetX() - x_begin) / x_width + ((int)_player->GetY() - y_begin) / y_width * x_count;
	if (grid < 0 || grid >= x_count * y_count)
	{
		return false;
	}
	m_grids[grid].m_player_list.push_back(_player);
	return true;
}

bool AOIWorld::Del_Player(Player* _player)
{
	int grid = ((int)_player->GetX() - x_begin) / x_width + ((int)_player->GetY() - y_begin) / y_width * x_count;
	std::find(m_grids[grid].m_player_list.begin(), m_grids[grid].m_player_list.end(), _player);

	/* -------------------------------------------------------------------------------------------------------- */
	m_grids[grid].m_player_list.erase(std::find(m_grids[grid].m_player_list.begin(), m_grids[grid].m_player_list.end(), _player));
	/* -------------------------------------------------------------------------------------------------------- */
}
