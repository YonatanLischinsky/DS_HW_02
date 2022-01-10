#ifndef LEVEL_H
#define LEVEL_H

#include "avl.h"
#include "group.h" //?
#include "HashTable.h"

#define MAXSCALE 200

class Group; //?

class Level
{
private:
    int level;
    int num_players;
    HashTable<Player> players;
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

    friend class Group; //?
    friend class Avl;
    friend class PlayersManager;
};


#endif