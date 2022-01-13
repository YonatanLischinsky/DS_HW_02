#ifndef GROUP_H
#define GROUP_H

#include "avl.h"
#include "player.h"
#include "level.h"


class Group
{
private:
    int id;
    int count; //players in group
    int count_levels; //at least 1 - level0
    bool level0_changed;
    std::shared_ptr<Avl> levels;
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
    StatusType  UpdateLevelHist(int level, int old_score, int new_score);
    //updates the level - decrease from hist in old_score, increase hist in new_score
    //also updates all the ranks in the path using UpdateRanks

    friend class Player;
    friend class PlayersManager;
};
#endif