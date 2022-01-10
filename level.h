#ifndef LEVEL_H
#define LEVEL_H

#include "avl.h"
#include "group.h" //?
#include "HashTable.h"

#define MAXSCALE 201

class Group; //?

class Level
{
private:
    int level;
    int num_players;
    HashTable<std::shared_ptr<Player>> players;
    int hist[MAXSCALE];

public:
    //C'tor
    Level(int lvl, int num) : level(lvl), num_players(num), players()
    { 
        for (int i = 0; i < MAXSCALE; i++)
        {
            hist[i] = 0;
        }
     } //C'tor

    Level(Level& p) = default; //Copy c'tor ??
    ~Level() = default;
    StatusType AddPlayer(std::shared_ptr<Player> player);
    StatusType RemovePlayer(int id);

    friend class Group; //?
    friend class Avl;
    friend class PlayersManager;
};


#endif