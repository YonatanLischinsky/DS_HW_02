#include "playersmanager.h"

PlayersManager::PlayersManager(int k_groups, int scale_input): k(k_groups), scale(scale_input)
{
    groups = std::shared_ptr<UnionFind<std::shared_ptr<Group>>>(new UnionFind<std::shared_ptr<Group>>(k_groups));
    all_players = std::shared_ptr<HashTable<std::shared_ptr<Player>>> (new HashTable<std::shared_ptr<Player>>());

    for (int i = 0; i <= k; i++)
    {
        groups->MakeSet(std::shared_ptr<Group>( new Group(i) ), i);
    }
}

StatusType PlayersManager::MergeGroups(int GroupID1, int GroupID2)
{
    StatusType st;
    StatusType res1, res2;
    std::shared_ptr<Group> g1 = groups->Find(GroupID1, &res1);
    std::shared_ptr<Group> g2 = groups->Find(GroupID2, &res2);
    if(res1 != SUCCESS || res2 != SUCCESS)
        return INVALID_INPUT;

    if(g1->id == g2->id) //already unioned
        return SUCCESS;
    
    std::shared_ptr<Group> destinationGroup;
    std::shared_ptr<Group> sourceGroup;
    destinationGroup = groups->Union(GroupID1, GroupID2, &st);
    sourceGroup = (destinationGroup != g1) ? g1 : g2;
    if(st != SUCCESS)
        return st;
    
    //~~~~~~~~~~~~~~~ to do - fill this function... ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~!@@$@$#@$~~~~~~~~~~~~~
    st = MoveDataFromSourceToDest(sourceGroup, destinationGroup); //O(N)
    return st;
}

StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int score)
{
    if(PlayerID <= 0 || GroupID <= 0 || GroupID > k || score <= 0 || score > scale)
        return INVALID_INPUT;

    StatusType st;
    all_players->Find(PlayerID, &st);
    if(st == SUCCESS) //player already exists!
        return FAILURE;
    
    std::shared_ptr<Player> to_insert = std::shared_ptr<Player>(new Player(PlayerID, 0, GroupID, score));
    std::shared_ptr<Group> g  = groups->Find(GroupID, &st);
    std::shared_ptr<Group> g0 = groups->Find(0, &st);

    /* **************** Insert to all_player Hash Table:  **************** */
    st = all_players->Insert(to_insert, PlayerID);
    if(st != SUCCESS)
        return st;

    /* **************** Insert to GroupID:  **************** */
    st = g->AddPlayer(to_insert);
    if(st != SUCCESS)
        return st;

    /* **************** Insert to group0:  **************** */
    st = g0->AddPlayer(to_insert);

    return st;
}

StatusType PlayersManager::RemovePlayer(int PlayerID)
{
    if(PlayerID <= 0)
        return INVALID_INPUT;
    
    StatusType st;
    std::shared_ptr<Player> to_remove = all_players->Find(PlayerID, &st);
    if (st == FAILURE)
        return FAILURE;
    
    //need to complete group.RemovePlayer
    groups->Find(0, &st)->RemovePlayer(PlayerID, to_remove->level); // O(1) always
    groups->Find(to_remove->group, &st)->RemovePlayer(PlayerID, to_remove->level);
    return SUCCESS;
}

StatusType PlayersManager::IncreasePlayerIDLevel(int PlayerID, int LevelIncrease)
{
    if(PlayerID <= 0 || LevelIncrease <= 0)
        return INVALID_INPUT;
    
    StatusType st;
    std::shared_ptr<Player> to_increase = all_players->Find(PlayerID, &st);
    if (st == FAILURE)
        return FAILURE;
    
    groups->Find(0, &st)->RemovePlayer(PlayerID, to_increase->level); // O(1) always
    groups->Find(to_increase->group, &st)->RemovePlayer(PlayerID, to_increase->level);
    to_increase->level += LevelIncrease;

    //need to complete group.AddPlayer
    groups->Find(0, &st)->AddPlayer(to_increase);
    if (st == ALLOCATION_ERROR)
        return ALLOCATION_ERROR;
    groups->Find(to_increase->group, &st)->AddPlayer(to_increase);
    if (st == ALLOCATION_ERROR)
        return ALLOCATION_ERROR;
    return SUCCESS;
}

StatusType PlayersManager::ChangePlayerIDScore(int PlayerID, int NewScore)
{
    if(PlayerID <= 0 || NewScore <= 0 || NewScore > scale)
        return INVALID_INPUT;
    
    StatusType st;
    std::shared_ptr<Player> to_edit = all_players->Find(PlayerID, &st);
    if (st == FAILURE)
        return FAILURE;
    
    if (to_edit->score == NewScore)
        return SUCCESS;

    int old_score = to_edit->score;
    to_edit->score = NewScore;
    StatusType st;

    //need to complete group.UpdateLevelHist
    groups->Find(0, &st)->UpdateLevelHist(to_edit->level, old_score, NewScore);

    return SUCCESS;
}

StatusType PlayersManager::GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,
                                            double * players);
StatusType PlayersManager::AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level);
StatusType PlayersManager::GetPlayersBound(int GroupID, int score, int m,
                                        int * LowerBoundPlayers, int * HigherBoundPlayers);


StatusType MoveDataFromSourceToDest(std::shared_ptr<Group> sourceGroup,
                                    std::shared_ptr<Group> destinationGroup)
{

}