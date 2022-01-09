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
    int left_hist[MAXSCALE];
    int right_hist[MAXSCALE];
    int num_players_left;
    int num_players_right;
    int sum_left;
    int sum_right;

public:
    Level(int lvl, int num) : level(lvl), num_players(num), num_players_left(0), num_players_right(0), sum_left(0), sum_right(0) 
    { 
        for (int i = 0; i < MAXSCALE; i++)
        {
            hist[i] = 0;
            left_hist[i] = 0;
            right_hist[i] = 0; 
        }
        //init hashtable
     } //C'tor

    Level(Level& p) = default; //Copy c'tor ??
    ~Level() = default; //need to free table

    friend class Group;
    friend class PlayersManager;
};


#endif