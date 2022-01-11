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
    std::shared_ptr<Group> g0 = groups->Find(0, &res1);
    if(g0->level0_changed)
    {
        g0->UpdateRanks(g0->level0);
        g0->level0_changed = false;
    }
    if(g1->level0_changed)
    {
        g1->UpdateRanks(g1->level0);
        g1->level0_changed = false;
    }
    if(g2->level0_changed)
    {
        g2->UpdateRanks(g2->level0);
        g2->level0_changed = false;
    }


    if(g1->id == g2->id) //already unioned
        return SUCCESS;
    
    std::shared_ptr<Group> destinationGroup;
    std::shared_ptr<Group> sourceGroup;
    destinationGroup = groups->Union(GroupID1, GroupID2, &st);
    sourceGroup = (destinationGroup != g1) ? g1 : g2;
    if(st != SUCCESS)
        return st;
    
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

    if(g0->level0_changed)
    {
        g0->UpdateRanks(g0->level0);
        g0->level0_changed = false;
    }
    if(g->level0_changed)
    {
        g->UpdateRanks(g->level0);
        g->level0_changed = false;
    }


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
    
    std::shared_ptr<Group> g  = groups->Find(to_remove->group, &st);
    std::shared_ptr<Group> g0 = groups->Find(0, &st);

    if(g0->level0_changed)
    {
        g0->UpdateRanks(g0->level0);
        g0->level0_changed = false;
    }
    if(g->level0_changed)
    {
        g->UpdateRanks(g->level0);
        g->level0_changed = false;
    }

    g0->RemovePlayer(PlayerID, to_remove->level); // O(1) always
    g->RemovePlayer(PlayerID, to_remove->level);
    return SUCCESS;
}

StatusType PlayersManager::IncreasePlayerIDLevel(int PlayerID, int LevelIncrease)
{
    if(PlayerID <= 0 || LevelIncrease <= 0)
        return INVALID_INPUT;
    
    StatusType st;
    std::shared_ptr<Player> to_increase = all_players->Find(PlayerID, &st);
    if (st != SUCCESS)
        return st;
    
    std::shared_ptr<Group> g  = groups->Find(to_increase->group, &st);
    std::shared_ptr<Group> g0 = groups->Find(0, &st);

    if(g0->level0_changed)
    {
        g0->UpdateRanks(g0->level0);
        g0->level0_changed = false;
    }
    if(g->level0_changed)
    {
        g->UpdateRanks(g->level0);
        g->level0_changed = false;
    }

    g0->RemovePlayer(PlayerID, to_increase->level); 
    g->RemovePlayer(PlayerID, to_increase->level);
    to_increase->level += LevelIncrease;

    st = g0->AddPlayer(to_increase);
    if (st != SUCCESS)
        return st;
    
    st = g->AddPlayer(to_increase);
    if (st != SUCCESS)
        return st;
    
    return SUCCESS;
}

StatusType PlayersManager::ChangePlayerIDScore(int PlayerID, int NewScore)
{
    if(PlayerID <= 0 || NewScore <= 0 || NewScore > scale)
        return INVALID_INPUT;
    
    StatusType st;
    std::shared_ptr<Player> to_edit = all_players->Find(PlayerID, &st);
    if (st != SUCCESS)
        return st;
    
    if (to_edit->score == NewScore)
        return SUCCESS;
    
    std::shared_ptr<Group> g  = groups->Find(to_edit->group, &st);
    std::shared_ptr<Group> g0 = groups->Find(0, &st);

    if(g0->level0_changed)
    {
        g0->UpdateRanks(g0->level0);
        g0->level0_changed = false;
    }
    if(g->level0_changed)
    {
        g->UpdateRanks(g->level0);
        g->level0_changed = false;
    }
    
    int old_score = to_edit->score;
    to_edit->score = NewScore;

    g0->UpdateLevelHist(to_edit->level, old_score, NewScore);
    g->UpdateLevelHist(to_edit->level, old_score, NewScore);

    return SUCCESS;
}

StatusType PlayersManager::GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,
                                            double * players)
{
    //check if valid
    StatusType st;
    std::shared_ptr<Group> g  = groups->Find(GroupID, &st);
    std::shared_ptr<Group> g0 = groups->Find(0, &st);

    if(g0->level0_changed)
    {
        g0->UpdateRanks(g0->level0);
        g0->level0_changed = false;
    }
    if(g->level0_changed)
    {
        g->UpdateRanks(g->level0);
        g->level0_changed = false;
    }

    //need to continue

    return SUCCESS;
}
StatusType PlayersManager::AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level)
{
    StatusType st;
    //need to check validation.
    std::shared_ptr<Group> g  = groups->Find(GroupID, &st);
    std::shared_ptr<Group> g0 = groups->Find(0, &st);

    if(g0->level0_changed)
    {
        g0->UpdateRanks(g0->level0);
        g0->level0_changed = false;
    }
    if(g->level0_changed)
    {
        g->UpdateRanks(g->level0);
        g->level0_changed = false;
    }
    
    return SUCCESS;
}

//Bonus
StatusType PlayersManager::GetPlayersBound(int GroupID, int score, int m,
                                        int * LowerBoundPlayers, int * HigherBoundPlayers) 
{
    StatusType st;
    //need to check validation.
    std::shared_ptr<Group> g  = groups->Find(GroupID, &st);
    std::shared_ptr<Group> g0 = groups->Find(0, &st);

    if(g0->level0_changed)
    {
        g0->UpdateRanks(g0->level0);
        g0->level0_changed = false;
    }
    if(g->level0_changed)
    {
        g->UpdateRanks(g->level0);
        g->level0_changed = false;
    }
    
    return SUCCESS;
}


StatusType PlayersManager::MoveDataFromSourceToDest(std::shared_ptr<Group> sourceGroup,
                                                    std::shared_ptr<Group> destinationGroup)
{
    destinationGroup->count += sourceGroup->count;
    sourceGroup->max_level = nullptr;
    sourceGroup->level0    = nullptr;
    
    /* Merge the treeeees */
    
    int nD = destinationGroup->count_levels;
    int nS = sourceGroup->count_levels;

    std::shared_ptr<Level>* arr1 = new std::shared_ptr<Level>[nD];
    if (arr1 == nullptr)
        return ALLOCATION_ERROR;

    std::shared_ptr<Level>* arr2 = new std::shared_ptr<Level>[nS];
    if (arr2 == nullptr)
    {
        delete[] arr1;
        return ALLOCATION_ERROR;
    }

    std::shared_ptr<Level>* arrMergedData = new std::shared_ptr<Level>[nD + nS];
    if (arrMergedData == nullptr)
    {
        delete[] arr1;
        delete[] arr2;
        return ALLOCATION_ERROR;
    }

    std::shared_ptr<Level>* arrMerged_Levels_Data = new std::shared_ptr<Level>[nD + nS];
    if (arrMerged_Levels_Data == nullptr)
    {
        delete[] arr1;
        delete[] arr2;
        delete[] arrMergedData;
        return ALLOCATION_ERROR;
    }

    int* arrMergedKey = new int[nD + nS];
    if (arrMergedKey == nullptr)
    {
        delete[] arr1;
        delete[] arr2;
        delete[] arrMergedData;
        delete[] arrMerged_Levels_Data;
        return ALLOCATION_ERROR;
    }

    (sourceGroup->levels)->InOrderFillArrData(arr1, nD, true);
    (destinationGroup->levels)->InOrderFillArrData(arr2, nS, true);

    //Merge the arrays into arr3 //O(n1+n2)
    MergeGroups_Helper(arr1, nD, arr2, nS, arrMergedData);
    int num_of_levels = nD + nS;

    //check if there are same levels and merge their tables
    int c = 0;
    for (int i = 0; i < nS + nD - 1; i++)
    {
        if (arrMergedData[i]->level == arrMergedData[i+1]->level)
        {
            //mergelLevels returns a pointer to the new level
            arrMerged_Levels_Data[c++] = MergeLevels(arrMergedData[i], arrMergedData[i+1]);
            arrMergedData[i+1]->players = nullptr;
            num_of_levels--;
        }
    }


    //Fill the key array:
    for (int i = 0; i < num_of_levels; i++)
    {
        arrMergedKey[i] = arrMerged_Levels_Data[i]->level;
    }

    int treeHeight = ceil(log2(num_of_levels + 1)) - 1;      // [log(n1+n2+1)]-1
    int countRemoveFromRight = (pow(2, treeHeight + 1) - 1) - (num_of_levels); // (2^(treeHeight+1)-1) - (n1+n2)

    std::shared_ptr<Avl> emptyTree (new Avl(treeHeight)); //O(nS + nD)
    emptyTree->reverseInOrderRemoveNodes(countRemoveFromRight);
    emptyTree->InOrderFillArrData(arrMerged_Levels_Data, num_of_levels, false); // //O(nS + nD)
    emptyTree->InOrderFillArrKey(arrMergedKey, num_of_levels, false); //O(nS + nD)

    destinationGroup->levels = emptyTree;
    destinationGroup->max_level = destinationGroup->levels->getData(destinationGroup->levels->GetMaxKey());

    delete[] arr1;
    delete[] arr2;
    delete[] arrMergedData;
    delete[] arrMerged_Levels_Data;
    delete[] arrMergedKey;

    return SUCCESS;
}

std::shared_ptr<Level> PlayersManager::MergeLevels(std::shared_ptr<Level> a, std::shared_ptr<Level> b)
{
    //run over B hashable:
    //  a.insert(b.pop);
    std::shared_ptr<HashTable<std::shared_ptr<Player>>> a_players = a->players;
    std::shared_ptr<HashTable<std::shared_ptr<Player>>> b_players = b->players;
    std::shared_ptr<HT_Node<std::shared_ptr<Player>>> * arr = b_players->GetArray();
    int size = b_players->GetSize();

    for (int i = 0; i < size; i++) // O(N) - average on the kelet
    {
        if(arr[i] == nullptr)
            continue;
        
        //run on chain:
        std::shared_ptr<HT_Node<std::shared_ptr<Player>>> ptr = arr[i];

        while(ptr != nullptr)
        {
            std::shared_ptr<Player> dataToInsert = ptr->data;
            a_players->Insert(dataToInsert, dataToInsert->id);
            a->num_players++;
            a->hist[dataToInsert->score]++;
            arr[i] = ptr->next;
            if (ptr->next != nullptr)
                ptr->next->prev = nullptr;
            ptr->next = nullptr;
            ptr->prev = nullptr;
            
            ptr = arr[i];
        }
    }
    return a;
}

void PlayersManager::MergeGroups_Helper(std::shared_ptr<Level> a[], int na, std::shared_ptr<Level> b[], int nb, std::shared_ptr<Level> c[])
{
    int ia = 0;
    int ib = 0;
    int ic = 0;
    while (ia < na && ib < nb)
    {
        int p1 = a[ia]->level;
        int p2 = b[ib]->level;
        //int p1(a[ia]->level, a[ia]->id);
        //int p2(b[ib]->level, b[ib]->id);

        if (p1 < p2)
        {
            c[ic++] = a[ia++];
        }
        else
        {
            c[ic++] = b[ib++];
        }
    }

    while (ia < na)
    {
        c[ic++] = a[ia++];
    }
    while (ib < nb)
    {
        c[ic++] = b[ib++];
    }
}