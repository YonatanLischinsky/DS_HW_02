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
    std::shared_ptr<UnionFind<std::shared_ptr<Group>>> groups;
    std::shared_ptr<HashTable<std::shared_ptr<Player>>> all_players;
    int k;
    int scale;

    /* Helper Functions: */
    StatusType MoveDataFromSourceToDest(std::shared_ptr<Group> sourceGroup,
        std::shared_ptr<Group> destinationGroup); //O(N) -> N = the amount of player in both groups.
    void MergeGroups_Helper(std::shared_ptr<Level> a[], int na, std::shared_ptr<Level> b[],
        int nb, std::shared_ptr<Level> c[]);
    std::shared_ptr<Level> MergeLevels(std::shared_ptr<Level> a, std::shared_ptr<Level> b);

public:
    PlayersManager(int k_groups, int scale_input); //C'tor
    ~PlayersManager() = default; //D'tor

    StatusType MergeGroups(int GroupID1, int GroupID2);
    StatusType AddPlayer(int PlayerID, int GroupID, int score);
    StatusType RemovePlayer(int PlayerID);
    StatusType IncreasePlayerIDLevel(int PlayerID, int LevelIncrease);
    StatusType ChangePlayerIDScore(int PlayerID, int NewScore);
    StatusType GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,
        double* players);
    StatusType AverageHighestPlayerLevelByGroup(int GroupID, int m, double* level);
    StatusType GetPlayersBound(int GroupID, int score, int m,
        int* LowerBoundPlayers, int* HigherBoundPlayers);

    void DEBUG_PrintLevelsTree(int groupID)
    {
        StatusType st;
        std::shared_ptr<Group> g = groups->Find(groupID, &st);
        g->levels->printInOrder(false);
    }
};

#endif