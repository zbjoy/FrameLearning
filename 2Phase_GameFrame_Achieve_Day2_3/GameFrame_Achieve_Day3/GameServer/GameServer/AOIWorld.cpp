#include "AOIWorld.h"

AOIWorld::AOIWorld(int _x_begin, int _x_end, int _y_begin, int _y_end, int _x_count, int _y_count) : x_begin(_x_begin), x_end(_x_end), y_begin(_y_begin), y_end(_y_end), x_count(_x_count), y_count(_y_count)
{
    x_width = (x_end - x_begin) / x_count;
    y_width = (y_end - y_begin) / y_count;
}

std::list<Player*> AOIWorld::GetSrdPlayer(Player* _player)
{
    return std::list<Player*>();
}

bool AOIWorld::Add_Player(Player* _player)
{
     
    return false;
}

bool AOIWorld::Del_Player(Player* _player)
{
    return false;
}
