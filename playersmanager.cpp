#include "playersmanager.h"

PlayersManager::PlayersManager(int k, int scale): groups(k), group0(0), all_players(), k(k), scale(scale)
{
    //how to initialize each group
}

StatusType PlayersManager::MergeGroups(int GroupID1, int GroupID2);
StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int score);
StatusType PlayersManager::RemovePlayer(int PlayerID);
StatusType PlayersManager::IncreasePlayerIDLevel(int PlayerID, int LevelIncrease);
StatusType PlayersManager::ChangePlayerIDScore(int PlayerID, int NewScore);
StatusType PlayersManager::GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,
                                            double * players);
StatusType PlayersManager::AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level);
StatusType PlayersManager::GetPlayersBound(int GroupID, int score, int m,
                                        int * LowerBoundPlayers, int * HigherBoundPlayers);