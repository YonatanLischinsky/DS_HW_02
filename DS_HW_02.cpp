#include <iostream>
#include <memory>
#include "UnionFind.h"
#include "HashTable.h"
#include "playersmanager.h"
#include "library2.h"
#include <string>

using namespace std;

#define N 5

string ResultCode(StatusType st)
{
	switch (st)
	{
	case SUCCESS:
		return "SUCCESS";

	case FAILURE:
		return "FAILURE";

	case ALLOCATION_ERROR:
		return "ALLOCATION ERROR";

	case INVALID_INPUT:
		return "INVALID INPUT";
	}

	return "No error code found.";
}

int main()
{
	cout << "Test started !" << endl << endl;
	StatusType st;

	PlayersManager pm = PlayersManager(N, 150);

	int g1 = 1;
	int g2 = 2;
	int increaseAmount = 5;
	double avg = -1;
	double percent = -1;
	int numPlayers = 100;

	cout << endl << "Inserting to Group1: " << endl << endl;

	for (int id = 1; id <= numPlayers; id++)
	{
		cout << "Adding player #" << id << " to Group #" << g1 << ".  result is : " << ResultCode(pm.AddPlayer(id, g1 , 100)) << endl;
	}

	cout << "printing in order the levels: "; 
	cout << endl;
	pm.DEBUG_PrintLevelsTree(g1);
	cout << endl;

	cout << endl << "Increasing levels: To g1 " << endl << endl;
	for (int id = 1; id <= numPlayers; id++)
	{
		cout << "Increase player #" << id << " Level to Level " << increaseAmount+id << ".  result is : " << ResultCode(pm.IncreasePlayerIDLevel(id, increaseAmount + id)) << endl;
	}

	cout << "printing in order the levels: ";
	cout << endl;
	pm.DEBUG_PrintLevelsTree(g1);
	cout << endl;
	
	cout << endl << "avg result is: " << ResultCode(pm.AverageHighestPlayerLevelByGroup(g1, 100, &avg)) << endl;
	cout << "The avg is: " << avg << endl << endl << endl;

	/*cout << endl << "Get..Percent (score=100) result is: " << ResultCode(pm.GetPercentOfPlayersWithScoreInBounds(g1, 100, 1, 100, &percent)) << endl;
	cout << "The percent is: (should be 100%)" << percent << "%" << endl << endl << endl;

	for (int id = 1; id <= 10; id++)
	{
		cout << "Changing score to id%2 + 1, to player " << id << "Result is  " << ResultCode(pm.ChangePlayerIDScore(id, (id%2) + 1)) << endl;
	}
	cout << endl << "Get..Percent (score=100) result is: " << ResultCode(pm.GetPercentOfPlayersWithScoreInBounds(g1, 1, 1, 100, &percent)) << endl;
	cout << "The percent is: " << percent << "%" << endl << endl << endl;
	*/
	/*
	cout << endl << "Inserting to Group2: " << endl << endl;

	for (int id = 16; id <= 30; id++)
	{
		cout << "Adding player #" << id << " to Group #" << g2 << ".  result is : " << ResultCode(pm.AddPlayer(id, g2, 100 + id)) << endl;
	}

	cout << endl << "Increasing levels: To g2 " << endl << endl;
	increaseAmount = 15;
	for (int id = 16; id <= 30; id++)
	{
		cout << "Increase player #" << id << " Level to Level " << increaseAmount << ".  result is : " << ResultCode(pm.IncreasePlayerIDLevel(id, increaseAmount)) << endl;
	}

	cout << endl << "Merging Groups: " << g1 << " " << g2 << "  result is: " << ResultCode(pm.MergeGroups(g1, g2)) << endl;
	
	cout << endl << "After the merge, the new Avg result is: " << ResultCode(pm.AverageHighestPlayerLevelByGroup(g2, 30, &avg)) << endl;
	cout << "The avg is: " << avg << endl << endl << endl;
	*/
	
	/*
	//for (int id = 1; id <= 30; id++)
	//{
	//	cout << "Removing player #" << id << ". result is: " << pm.RemovePlayer(id) << endl;
	//}*/

	cout << "Test Finished !" << endl;


}

