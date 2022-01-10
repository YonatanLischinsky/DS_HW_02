#ifndef GROUP_H
#define GROUP_H

#include "avl.h"
#include "player.h"
#include "level.h"

class Player;

class Group
{
private:
    int id;
    std::shared_ptr < Avl<std::shared_ptr<Level>, int> > levels;
    int count;
    std::shared_ptr<Level> level0;
    std::shared_ptr<Level> max_level;
    StatusType AddLevel(int level); 
    StatusType RemoveLevel(int level);
    StatusType UpdateRanks(std::shared_ptr<Level> level);

public:
    Group(int id); //C'tor, need to add initializing of levels tree with level0
    ~Group()
    {
        levels->deleteAllTree();
        levels = nullptr;
        level0 = nullptr;
        max_level = nullptr;
    }
    StatusType AddPlayer(std::shared_ptr<Player> player); 
    StatusType RemovePlayer(int id, int level);

    friend class Player;
    friend class PlayersManager;
};
#endif