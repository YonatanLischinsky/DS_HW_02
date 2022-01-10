#ifndef PLAYER_H
#define PLAYER_H

#include "avl.h"

class Player
{
private:
    int id;
    int level;
    int group;

public:
    Player(int id, int lvl, int g) : id(id), level(lvl),group(g)
    { } //C'tor

    Player(Player& p) = default; //Copy c'tor
    ~Player() = default;

    friend class Group;
    friend class PlayersManager;
};

#endif