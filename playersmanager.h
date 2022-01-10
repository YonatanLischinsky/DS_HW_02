#ifndef PLAYERSMANAGER_H
#define PLAYERSMANAGER_H

#include "library2.h"
#include "group.h"
#include "UnionFind.h"
#include "HashTable.h"
#include "player.h"

class PlayersManager
{
    private:
        UnionFind<Group> groups;
        Group group0;
        HashTable<Player> all_players;
        int k;
        int scale;

    public:
        PlayersManager(int k, int scale); //C'tor
        ~PlayersManager() = default; //D'tor
        StatusType MergeGroups(int GroupID1, int GroupID2);
        StatusType AddPlayer(int PlayerID, int GroupID, int score);
        StatusType RemovePlayer(int PlayerID);
        StatusType IncreasePlayerIDLevel(int PlayerID, int LevelIncrease);
        StatusType ChangePlayerIDScore(int PlayerID, int NewScore);
        StatusType GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,
                                                    double * players);
        StatusType AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level);
        StatusType GetPlayersBound(int GroupID, int score, int m,
                                                int * LowerBoundPlayers, int * HigherBoundPlayers);
};

#endif