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
    int count;
    bool level0_changed;
    std::shared_ptr<Avl> levels;
    std::shared_ptr<Level> level0;
    std::shared_ptr<Level> max_level;

    StatusType AddLevel(int level);
    StatusType UpdateRanks(std::shared_ptr<Level> level);
    StatusType RemoveLevel(int level);

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
    StatusType UpdateLevelHist(int level, int old_score, int new_score);
    //update level hist and all the levels above (ranks)

    friend class Player;
    friend class PlayersManager;
};
#endif