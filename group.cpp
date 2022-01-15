#include "group.h"

Group::Group(int id) : id(id), count(0), count_levels(1), level0_changed(false)
{
    levels = std::shared_ptr<Avl>(new Avl());
    level0 = std::shared_ptr<Level>(new Level(0, 0));
    max_level = level0;
    levels->insert(level0, 0);
}

StatusType Group::AddLevel(int level)
{
    StatusType res;
    if (level <= 0)
        return INVALID_INPUT;

    std::shared_ptr<Level> lvl_to_insert = std::shared_ptr<Level>(new Level(level, 0));
    if (lvl_to_insert == nullptr)
        return ALLOCATION_ERROR;

    if (level > max_level->level)
        max_level = lvl_to_insert;
    res = levels->insert(lvl_to_insert, level);
    if (res != SUCCESS)
        return res;
    count_levels++;
    return res;
}

StatusType Group::RemoveLevel(int level)
{
    if (level <= 0)
        return INVALID_INPUT;

    StatusType res;
    res = levels->remove(level);
    if (res != SUCCESS)
        return res;

    count_levels--;
    if (level == max_level->level)
    {
        max_level = levels->getData(levels->GetMaxKey());
    }

    return res;
}

StatusType Group::UpdateRanks(std::shared_ptr<Level> level)
{
    if (level == nullptr)
        return FAILURE;
    levels->UpdateRankedData(level->level);
    return SUCCESS;
}

StatusType Group::AddPlayer(std::shared_ptr<Player> player)
{
    StatusType res;
    if (player->level == 0)
    {
        res = level0->AddPlayer(player);
        if (res != SUCCESS)
            return res;
        level0_changed = true;
    }
    else
    {
        std::shared_ptr<Level> lvl = levels->getData(player->level);
        if (lvl != nullptr)
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
    if (level == 0)
    {
        res = level0->RemovePlayer(id);
        UpdateRanks(level0);
    }
    else
    {
        std::shared_ptr<Level> lvl = levels->getData(level);
        res = lvl->RemovePlayer(id);
        if (lvl->num_players == 0)
            RemoveLevel(level);
        else
            UpdateRanks(lvl);
    }
    count--;
    return SUCCESS;
}

StatusType Group::UpdateLevelHist(int level, int old_score, int new_score)
{
    std::shared_ptr<Level> lvl = levels->getData(level);
    lvl->hist[old_score]--;
    lvl->hist[new_score]++;
    return UpdateRanks(lvl);
}