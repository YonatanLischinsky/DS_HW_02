#ifndef LEVEL_H
#define LEVEL_H

#include "HashTable.h"
#include "player.h"

#define MAXSCALE 201

class Level
{
private:
    int level;
    int num_players;
    std::shared_ptr<HashTable<std::shared_ptr<Player>>> players;
    int hist[MAXSCALE];

public:
    Level(int lvl, int num); //C'tor
    Level(Level& p) = default; //Copy c'tor
    ~Level() = default;
    StatusType AddPlayer(std::shared_ptr<Player> player);
    StatusType RemovePlayer(int id);

    friend class Group; //?
    friend class Avl;
    friend class PlayersManager;
};


#endif