#include "library2.h"
#include "playersmanager.h"


void* Init(int k, int scale)
{
    if (k <= 0 || scale <= 0 || scale >= MAXSCALE)
    PlayersManager *DS = new PlayersManager(k, scale);
    return ((void*)DS);
}

StatusType MergeGroups(void* DS, int GroupID1, int GroupID2)
{
    if (DS == nullptr)
        return INVALID_INPUT;
    return ((PlayersManager*)DS)->MergeGroups(GroupID1, GroupID2);
}

StatusType AddPlayer(void* DS, int PlayerID, int GroupID, int score)
{
    if (DS == nullptr)
        return INVALID_INPUT;
    return ((PlayersManager*)DS)->AddPlayer(PlayerID, GroupID, score);
}

StatusType RemovePlayer(void* DS, int PlayerID)
{
    if (DS == nullptr)
        return INVALID_INPUT;
    return ((PlayersManager*)DS)->RemovePlayer(PlayerID);
}

StatusType IncreasePlayerIDLevel(void* DS, int PlayerID, int LevelIncrease)
{
    if (DS == nullptr)
        return INVALID_INPUT;
    return ((PlayersManager*)DS)->IncreasePlayerIDLevel(PlayerID, LevelIncrease);
}

StatusType ChangePlayerIDScore(void* DS, int PlayerID, int NewScore)
{
    if (DS == nullptr)
        return INVALID_INPUT;
    return ((PlayersManager*)DS)->ChangePlayerIDScore(PlayerID, NewScore);
}

StatusType GetPercentOfPlayersWithScoreInBounds(void* DS, int GroupID, int score, int lowerLevel, int higherLevel,
    double* players)
{
    if (DS == nullptr)
        return INVALID_INPUT;
    return ((PlayersManager*)DS)->GetPercentOfPlayersWithScoreInBounds(GroupID, score, lowerLevel, higherLevel, players);
}

StatusType AverageHighestPlayerLevelByGroup(void* DS, int GroupID, int m, double* level)
{
    if (DS == nullptr)
        return INVALID_INPUT;
    return ((PlayersManager*)DS)->AverageHighestPlayerLevelByGroup(GroupID, m, level);
}

//Bonus
StatusType GetPlayersBound(void* DS, int GroupID, int score, int m,
    int* LowerBoundPlayers, int* HigherBoundPlayers)
{
    if (DS == nullptr)
        return INVALID_INPUT;
    return ((PlayersManager*)DS)->GetPlayersBound(GroupID, score, m, LowerBoundPlayers, HigherBoundPlayers);
}

void Quit(void** DS) {
    if (DS == nullptr)
        return ;
    delete (*((PlayersManager**)DS)); //dereference and cast
    *DS = nullptr;
}
