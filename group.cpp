#include "group.h"

Group::Group(int id) : id(id), count(0), level0_changed(false)
{
    levels = std::shared_ptr<Avl> (new Avl());
    level0 = std::shared_ptr<Level> (new Level(0, 0));
    max_level = level0;
    levels->insert(level0, 0);
}

StatusType Group::AddLevel(int level)
{
    StatusType res;
    if(level <= 0)
        return INVALID_INPUT;
    
    std::shared_ptr<Level> lvl_to_insert = std::shared_ptr<Level>(new Level(level, 0));
    if(lvl_to_insert == nullptr)
        return ALLOCATION_ERROR;

    return (levels->insert(lvl_to_insert, level));
}

StatusType Group::RemoveLevel(int level)
{
    if(level <= 0)
        return INVALID_INPUT;
    StatusType res;
    res = levels->remove(level);
    
    //Todo: handle the ranking data.
    return res;
}

// StatusType Group::UpdateRanks(std::shared_ptr<Level> level);

StatusType Group::AddPlayer(std::shared_ptr<Player> player)
{
    StatusType res;
    if(player->level == 0)
    {
        res = level0->AddPlayer(player);
        if (res != SUCCESS)
            return res;
        level0_changed = true;
    }
    else
    {
        std::shared_ptr<Level> lvl = levels->getData(player->level);
        if(lvl != nullptr)
        {
            res = lvl->AddPlayer(player);
            if (res != SUCCESS)
                return res;
        }
        else
        {
            res = AddLevel(player->level);
            if (res != SUCCESS)
                return res;
            lvl = levels->getData(player->level);
            res = lvl->AddPlayer(player);
            if (res != SUCCESS)
                return res;
        }
        levels->UpdateRankedData(player->level);
    }
    count++;
    return SUCCESS;
}

StatusType Group::RemovePlayer(int id, int level)
{
    StatusType res;
    if(level == 0)
    {
        res = level0->RemovePlayer(id);
        level0_changed = true;
    }
    else
    {
        std::shared_ptr<Level> lvl = levels->getData(level);
        res = lvl->RemovePlayer(id);
        if (lvl->num_players == 0)
            RemoveLevel(level);
    }
    count--;
    return SUCCESS;
}
//StatusType Group::UpdateLevelHist(int level, int old_score, int new_score);

/*
StatusType Group::AddPlayer(std::shared_ptr<Player> p, Pair key)
{
    StatusType st = players->insert(p, key);
    if (st == SUCCESS)
    {
        count++;
        if ((key.levels > max_level) || (key.levels == max_level && key.id < id_max_level))
        {
            max_level = key.levels;
            id_max_level = key.id;
        }
    }

    return st;
}

StatusType Group::RemovePlayer(Pair p)
{
    StatusType st = players->remove(p);
    if (st == SUCCESS)
    {
        count--;
        if (count == 0)
        {
            max_level = -1;
            id_max_level = -1;
        }
        else if (p.id == id_max_level)
        {
            Pair max = players->GetMaxKey();
            max_level = max.levels;
            id_max_level = max.id;
        }
    }
    return st;
}
*/