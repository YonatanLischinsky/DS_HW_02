#ifndef AVL_H
#define AVL_H

using namespace std;
#include <memory>
#include "library2.h"
#include "level.h"

#define MAXSCALE 201

class Avl;

typedef enum
{
    LEFT = 0,
    RIGHT
} INSERT_DIRECTION;

typedef enum
{
    LL = 0,
    LR,
    RL,
    RR
} GILGOL_TYPE;


class Node
{
public:
    Node(int h) : balance_factor(0), height(h), data(nullptr), left(nullptr), right(nullptr), father(nullptr)
    {
        key = 0;
        balance_factor = 0;
        height = 0;
        min_left = 0;
        max_right = 0;
        num_players_left = 0;
        num_players_right = 0;
        sum_left = 0;
        sum_right = 0;

        for (int i = 0; i < MAXSCALE; i++)
        {
            left_hist[i] = 0;
            right_hist[i] = 0;
        }
    }
    Node(std::shared_ptr<Level> data, int key); //Constructor
    Node(const Node& node); //copy constructor
    ~Node(); //destructor

    std::shared_ptr<Level> data;
    int key;

    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    std::shared_ptr<Node> father;

    //ranks
    int balance_factor;
    int height;

    //need to add functions that update the ranks below with every insert/remove
    int min_left;
    int max_right;
    int left_hist[MAXSCALE];
    int right_hist[MAXSCALE];
    int num_players_left;
    int num_players_right;
    int sum_left;
    int sum_right;

    void UpdateBF();
    void UpdateHeight();
    int max(int a, int b); //only used for calculating heights - keep in int !

    friend class Avl;
};

class Avl
{
public:
    Avl(); //constructor
    Avl(int height); //constructor to create FLUL-EMPTY tree. (for merge function)
    ~Avl(); //destructor

    StatusType insert(std::shared_ptr<Level> data, int key);
    StatusType remove(int key);
    int GetMaxKey();
    std::shared_ptr<Level> getData(int key);
    void printInOrder(bool reverse);
    void reverseInOrderFillArr(int arr[]);
    void InOrderFillArrData(std::shared_ptr<Level> arr[], int counter, bool fillArray);
    void InOrderFillArrKey(int arr[], int counter, bool fillArray);
    void reverseInOrderRemoveNodes(int count);
    void deleteAllTree();
    bool search(int key);
    void UpdateRankedData(int startingPoint_Key);
    void PostOrderUpdateRanks();
    double GetAverage_m_players(int m_players);
    double GetPercentOfPlayersWithScoreInBounds(int score, int lowerLevel, int higherLevel);
    std::shared_ptr<Node> FindBiggerThan(int key);
    std::shared_ptr<Node> FindSmallerThan(int key);

private:
    std::shared_ptr<Node> root;

    //private helper functions:

    void AfterInsertCheckTree(std::shared_ptr<Node> v);
    void Gilgol(std::shared_ptr<Node> r, GILGOL_TYPE type);
    std::shared_ptr<Node> binarySearch(int insert_key, std::shared_ptr<Node> sub_root, INSERT_DIRECTION* dir);
    void recInOrder(std::shared_ptr<Node> r, bool reverse);
    void deleteTree(std::shared_ptr<Node> r);
    int recGetMaxKey(std::shared_ptr<Node> n);
    std::shared_ptr<Node> findNode(std::shared_ptr<Node> sub_root, int key);
    void HandleOneSonRemove(std::shared_ptr<Node> nodeToRemove);
    std::shared_ptr<Node> binaryTreeRemoveAlgo(std::shared_ptr<Node> nodeToRemove);
    void reverseInOrderFillArr_rec(int arr[], std::shared_ptr<Node> n, int* i);
    void InOrderFillArrData_rec(std::shared_ptr<Level> arr[], std::shared_ptr<Node> n, int* i, int count, bool fillArray);
    void InOrderFillArrKey_rec(int arr[], std::shared_ptr<Node> n, int* i, int count, bool fillArray);
    void CreateEmptyFullTree_rec(int height, std::shared_ptr<Node> n);
    void reverseInOrderRemoveNodes_rec(std::shared_ptr<Node> n, int* i, int count);
    void UpdateRankedData_Right(std::shared_ptr<Node> startNode);
    void UpdateRankedData_Left(std::shared_ptr<Node> startNode);
    void UpdateRankedData_ByNode(std::shared_ptr<Node> startNode);
    void PostOrderUpdateRanks_rec(std::shared_ptr<Node> node);
    std::shared_ptr<Node> findSubroot(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2);

};

#endif