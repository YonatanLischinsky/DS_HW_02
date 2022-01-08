#ifndef GROUP_H
#define GROUP_H

#include "avl.h"
#include "player.h"
#include "level.h"
#include "pair.h"

class Player;

class Group
{
private:
    int id;
    //int max_level;
    //int id_max_level;
    std::shared_ptr < Avl<std::shared_ptr<Level>, int> > levels;
    int count;
    std::shared_ptr<Level> level0;

public:
    Group(int id); //C'tor, need to add initializing of levels tree with level0
    ~Group()
    {
        levels->deleteAllTree();
        levels = nullptr;
        level0 = nullptr;
    }
    StatusType AddPlayer(std::shared_ptr<Player> p, Pair key); //change to AddLevel
    StatusType RemovePlayer(Pair p); //change to RemoveLevel

    friend class Player;
    friend class PlayersManager;
};
#endif