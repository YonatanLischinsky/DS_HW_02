#include "level.h"

Level::Level(int lvl, int num) : level(lvl), num_players(num)
{ 
    for (int i = 0; i < MAXSCALE; i++)
    {
        hist[i] = 0;
    }
    players = std::shared_ptr<HashTable<std::shared_ptr<Player>>> (new HashTable<std::shared_ptr<Player>>);
} //C'tor

StatusType Level::AddPlayer(std::shared_ptr<Player> player)
{
    StatusType st = players->Insert(player, player->level);
    if (st != SUCCESS)
        return st;
    hist[player->score]++;
    num_players++;
    return SUCCESS;
}

StatusType Level::RemovePlayer(int id)
{
    StatusType st;
    std::shared_ptr<Player> p = players->Find(id, &st);
    if(st != SUCCESS)
        return st;
        
    hist[p->score] --;
    this->num_players --;
    return players->Remove(id);
}