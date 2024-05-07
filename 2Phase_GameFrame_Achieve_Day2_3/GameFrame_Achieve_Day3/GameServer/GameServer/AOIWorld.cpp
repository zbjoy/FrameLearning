#include "AOIWorld.h"

AOIWorld::AOIWorld(int _x_begin, int _x_end, int _y_begin, int _y_end, int _x_count, int _y_count) : x_begin(_x_begin), x_end(_x_end), y_begin(_y_begin), y_end(_y_end), x_count(_x_count), y_count(_y_count)
{
    x_width = (x_end - x_begin) / x_count;
    y_width = (y_end - y_begin) / y_count;

    for (int i = 0; i < x_count * y_count; i++)
    {
        // std::list<Grid> tmp;
        Grid tmp;
        m_grids.push_back(tmp);
    }
}

std::list<Player*> AOIWorld::GetSrdPlayer(Player* _player)
{
    std::list<Player*> ret_list;
    int x = (int)_player->GetX();
    int y = (int)_player->GetY();
    int grid = (x - x_begin) / x_width + (y - y_begin) / y_width * x_count;

    int x_index = (x - x_begin) / x_width;
    int y_index = (y - y_begin) / y_width;

    if (x_index > 0 && y_index > 0)
    {
        std::list<Player*>& cur_list = m_grids[grid - 1 - x_count].m_player_list;
        ret_list.insert(ret_list.end(), cur_list.begin(), cur_list.end());
    }
    if (y_index > 0)
    {
        std::list<Player*>& cur_list = m_grids[grid - x_count].m_player_list;
        ret_list.insert(ret_list.end(), cur_list.begin(), cur_list.end());
    }
    if (y_index > 0 && x_index < x_count - 1)
    {
        std::list<Player*>& cur_list = m_grids[grid + 1 - x_count].m_player_list;
        ret_list.insert(ret_list.end(), cur_list.begin(), cur_list.end());
    }
    if (x_index > 0)
    {
        std::list<Player*>& cur_list = m_grids[grid - 1].m_player_list;
        ret_list.insert(ret_list.end(), cur_list.begin(), cur_list.end());
    }
    std::list<Player*>& cur_list = m_grids[grid].m_player_list;
    ret_list.insert(ret_list.end(), cur_list.begin(), cur_list.end());
    if (x_index < x_count - 1)
    {
        std::list<Player*>& cur_list = m_grids[grid + 1].m_player_list;
        ret_list.insert(ret_list.end(), cur_list.begin(), cur_list.end());
    }
    if (x_index > 0 && y_index < y_count - 1)
    {
        std::list<Player*>& cur_list = m_grids[grid - 1 + x_count].m_player_list;
        ret_list.insert(ret_list.end(), cur_list.begin(), cur_list.end());
    }
    if (y_index < y_count - 1)
    {
        std::list<Player*>& cur_list = m_grids[grid + x_count].m_player_list;
        ret_list.insert(ret_list.end(), cur_list.begin(), cur_list.end());
    }
    if (x_index < x_count - 1 && y_index < y_count - 1)
    {
        std::list<Player*>& cur_list = m_grids[grid + 1 + x_count].m_player_list;
        ret_list.insert(ret_list.end(), cur_list.begin(), cur_list.end());
    }
    return ret_list;
}

bool AOIWorld::Add_Player(Player* _player)
{
    int x = (int)_player->GetX();
    int y = (int)_player->GetY();
    int grid = (x - x_begin) / x_width + (y - y_begin) / y_width * x_count;
    m_grids[grid].m_player_list.push_back(_player);
    return true;
}

bool AOIWorld::Del_Player(Player* _player)
{
    int x = (int)_player->GetX();
    int y = (int)_player->GetY();
    int grid = (x - x_begin) / x_width + (y - y_begin) / y_width * x_count;
    m_grids[grid].m_player_list.remove(_player);
    return true;
}
