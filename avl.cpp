#include <memory>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "avl.h"

class Avl;

/* *************************** Node Implementation Below *****************************/
/* Node Constructor */
Node::Node(std::shared_ptr<Level> data, int key) : data(data), key(key), left(nullptr), right(nullptr), father(nullptr),
                                                    balance_factor(0), height(0), min_left(key), max_right(key), 
                                                    num_players_left(0), num_players_right(0), sum_left(0), sum_right(0)
{
    for (int i = 0; i < MAXSCALE; i++)
    {
        left_hist[i] = 0;
        right_hist[i] = 0;
    }
}

/* Node Copy constructor */
Node::Node(const Node& node) : data(node.data), key(node.key), left(node.left), right(node.right), father(node.father)
{
}

/* Destructor */
Node::~Node()
{
}

void Node::UpdateBF()
{
    int hL, hR;
    if (this->left == nullptr)
        hL = -1;
    else
        hL = this->left->height;

    if (this->right == nullptr)
        hR = -1;
    else
        hR = this->right->height;

    this->balance_factor = hL - hR;
}

void Node::UpdateHeight()
{
    int Hl = -1, Hr = -1;
    if (this->left == nullptr)
        Hl = -1;
    else
        Hl = this->left->height;

    if (this->right == nullptr)
        Hr = -1;
    else
        Hr = this->right->height;

    this->height = max(Hl, Hr) + 1;
    this->UpdateBF();
}

int Node::max(int a, int b)
{
    return (a >= b ? a : b);
}

/* *************************** Avl Implementation Below *****************************/
/* AVL Constructor */
Avl::Avl(): root(nullptr)
{
}

/* AVL Constructor */
Avl::Avl(int height) : root(nullptr)
{
    std::shared_ptr<Node> r(new Node(height));
    root = r;
    CreateEmptyFullTree_rec(height, root);
}

void Avl::CreateEmptyFullTree_rec(int height, std::shared_ptr<Node> n)
{
    if (height == 0 || n == nullptr)
    {
        return;
    }

    std::shared_ptr<Node> leftSon(new Node(height - 1));
    std::shared_ptr<Node> rightSon(new Node(height - 1));

    leftSon->father = n;
    rightSon->father = n;

    n->left = leftSon;
    n->right = rightSon;


    CreateEmptyFullTree_rec(height - 1, leftSon);
    CreateEmptyFullTree_rec(height - 1, rightSon);
}

/* Destructor */
Avl::~Avl()
{
    deleteTree(this->root);
    root = nullptr;
}

void Avl::deleteAllTree()
{
    deleteTree(root);
    root = nullptr;
}

void Avl::deleteTree(std::shared_ptr<Node> r)
{
    if (r == nullptr)
        return;

    deleteTree(r->left);
    deleteTree(r->right);

    r->father = nullptr;
    r->left = nullptr;
    r->right = nullptr;
}

void Avl::printInOrder(bool reverse)
{
    recInOrder(this->root, reverse);
    std::cout << std::endl;
}

void Avl::recInOrder(std::shared_ptr<Node> r, bool reverse)
{
    if (r == nullptr)
        return;
    if (!reverse)
        recInOrder(r->left, reverse);
    else
        recInOrder(r->right, reverse);
    std::cout << r->key << " ";
    if (!reverse)
        recInOrder(r->right, reverse);
    else
        recInOrder(r->left, reverse);
}

StatusType Avl::insert(std::shared_ptr<Level> data, int key)
{
    std::shared_ptr<Node> nodeToInsert(new Node(data, key));
    if (nodeToInsert == nullptr)
        throw std::bad_alloc();

    if (this->root == nullptr)
    {
        this->root = nodeToInsert;
        return SUCCESS;
    }

    //Check if already exists:
    INSERT_DIRECTION dir;
    std::shared_ptr<Node> father = binarySearch(key, this->root, &dir);
    if (father == nullptr) //Key already exists !
    {
        return FAILURE;
    }

    if (dir == LEFT)
    {
        father->left = nodeToInsert;
    }
    else
    {
        father->right = nodeToInsert;
    }
    nodeToInsert->father = father;

    //Make gilgols:
    AfterInsertCheckTree(nodeToInsert);

    return SUCCESS;
}

StatusType Avl::remove(int key)
{
    std::shared_ptr<Node> nodeToRemove = findNode(root, key);
    if (nodeToRemove == nullptr)
        return FAILURE;

    std::shared_ptr<Node> father = binaryTreeRemoveAlgo(nodeToRemove);
    if (father == nullptr)
        return SUCCESS;

    int initHeight = father->height;
    while (father != nullptr)
    {
        father->UpdateHeight();

        //Do Gilgol:
        switch (father->balance_factor)
        {
        case 2:
            if (father->left->balance_factor >= 0)
            {
                Gilgol(father, LL);
            }
            else if (father->left->balance_factor == -1) //maybe else here ?...
            {
                Gilgol(father, LR);
            }
            break;

        case -2:
            if (father->right->balance_factor <= 0)
            {
                Gilgol(father, RR);
            }
            else if (father->right->balance_factor == 1) //maybe else here ?...
            {
                Gilgol(father, RL);
            }
            break;

        default:
            break;
        }

        if (initHeight == father->height)
            break;
        father = father->father;
    }

    return SUCCESS;
}

std::shared_ptr<Node> Avl::binaryTreeRemoveAlgo(std::shared_ptr<Node> nodeToRemove)
{
    if (nodeToRemove->height == 0)// leaf
    {
        if (root == nodeToRemove)
        {
            deleteTree(root);
            root = nullptr;
            return nullptr;
        }

        if (nodeToRemove->father->left == nodeToRemove)
            nodeToRemove->father->left = nullptr;
        else
            nodeToRemove->father->right = nullptr;

        return nodeToRemove->father;
    }
    else //not a leaf.
    {
        int countSons = 0;
        countSons += (nodeToRemove->left != nullptr);
        countSons += (nodeToRemove->right != nullptr);
        if (countSons == 1)
        {
            HandleOneSonRemove(nodeToRemove);
            return nodeToRemove->father;
        }
        else //2 sons
        {
            std::shared_ptr<Node> w = nodeToRemove->right;
            while (w->left != nullptr)
            {
                w = w->left;
            }

            //swap between nodeToRemove and w.
            std::shared_ptr<Level> tempData = nodeToRemove->data;
            int tempKey = nodeToRemove->key;

            nodeToRemove->key = w->key;
            nodeToRemove->data = w->data;

            w->key = tempKey;
            w->data = tempData;

            return binaryTreeRemoveAlgo(w);
        }
    }
}

void Avl::HandleOneSonRemove(std::shared_ptr<Node> nodeToRemove)
{
    if (nodeToRemove->left == nullptr && nodeToRemove->right != nullptr)
    {
        if (nodeToRemove->father != nullptr)
        {
            if (nodeToRemove->father->left == nodeToRemove)
                nodeToRemove->father->left = nodeToRemove->right;
            else
                nodeToRemove->father->right = nodeToRemove->right;
        }
        nodeToRemove->right->father = nodeToRemove->father;
        if (root == nodeToRemove)
            root = nodeToRemove->right;
    }
    else if (nodeToRemove->left != nullptr && nodeToRemove->right == nullptr)
    {
        if (nodeToRemove->father != nullptr)
        {
            if (nodeToRemove->father->left == nodeToRemove)
                nodeToRemove->father->left = nodeToRemove->left;
            else
                nodeToRemove->father->right = nodeToRemove->left;
        }
        nodeToRemove->left->father = nodeToRemove->father;

        if (root == nodeToRemove)
            root = nodeToRemove->left;

    }

    nodeToRemove->left = nullptr;
    nodeToRemove->right = nullptr;
    nodeToRemove->father = nullptr;

}

void Avl::AfterInsertCheckTree(std::shared_ptr<Node> v)
{
    while (v != this->root)
    {
        std::shared_ptr<Node> p = v->father;
        if (p->height >= v->height + 1)
            return;

        p->height = v->height + 1;
        p->UpdateBF();

        switch (p->balance_factor)
        {
        case 2:
            if (p->left->balance_factor >= 0)
            {
                Gilgol(p, LL);
            }
            else if (p->left->balance_factor == -1) //maybe else here ?...
            {
                Gilgol(p, LR);
            }
            return;

        case -2:
            if (p->right->balance_factor <= 0)
            {
                Gilgol(p, RR);
            }
            else if (p->right->balance_factor == 1) //maybe else here ?...
            {
                Gilgol(p, RL);
            }
            return;

        default: //BF is ok
            UpdateRankedData_Left(p);
            UpdateRankedData_Right(p);
            v = p;
            
            break;
        }
    }
}

void Avl::Gilgol(std::shared_ptr<Node> r, GILGOL_TYPE type)
{
    switch (type)
    {
    case LL:
    {
        std::shared_ptr<Node> tempA = r->left;

        r->left = tempA->right;
        if (tempA->right != nullptr)
            tempA->right->father = r;
        tempA->right = r;
        tempA->father = r->father;

        if (r->father != nullptr)
        {
            if (r->father->right == r)
            {
                r->father->right = tempA;
            }
            else
            {
                r->father->left = tempA;
            }
        }
        r->father = tempA;


        r->UpdateHeight();
        tempA->UpdateHeight();

        if (this->root == r)
            this->root = tempA;
        
        UpdateRankedData_ByNode(r);
    }
    break;

    case RR:
    {
        std::shared_ptr<Node> tempA = r->right;

        r->right = tempA->left;
        if (tempA->left != nullptr)
            tempA->left->father = r;
        tempA->left = r;
        tempA->father = r->father;

        if (r->father != nullptr)
        {
            if (r->father->right == r)
            {
                r->father->right = tempA;
            }
            else
            {
                r->father->left = tempA;
            }
        }
        r->father = tempA;

        r->UpdateHeight();
        tempA->UpdateHeight();

        if (this->root == r)
            this->root = tempA;

        UpdateRankedData_ByNode(r);
    }
    break;

    case LR:
    {
        std::shared_ptr<Node> tempA = r->left;
        std::shared_ptr<Node> tempB = r->left->right;
        r->left = tempB->right;
        if (tempB->right != nullptr)
            tempB->right->father = r;

        tempA->right = tempB->left;
        if (tempB->left != nullptr)
            tempB->left->father = tempA;

        tempB->left = tempA;
        tempA->father = tempB;

        tempB->right = r;

        if (r->father != nullptr)
        {
            if (r->father->right == r)
            {
                r->father->right = tempB;
            }
            else
            {
                r->father->left = tempB;
            }
        }
        tempB->father = r->father;
        r->father = tempB; //Keep this under the if/else

        tempA->UpdateHeight();
        r->UpdateHeight();
        tempB->UpdateHeight();

        if (r == this->root)
            this->root = tempB;
        UpdateRankedData_ByNode(tempA);
        UpdateRankedData_ByNode(r);
    }
    break;

    case RL:
    {
        std::shared_ptr<Node> tempA = r->right;
        std::shared_ptr<Node> tempB = r->right->left;
        r->right = tempB->left;
        if (tempB->left != nullptr)
            tempB->left->father = r;

        tempA->left = tempB->right;
        if (tempB->right != nullptr)
            tempB->right->father = tempA;

        tempB->right = tempA;
        tempA->father = tempB;

        tempB->left = r;

        if (r->father != nullptr)
        {
            if (r->father->right == r)
            {
                r->father->right = tempB;
            }
            else
            {
                r->father->left = tempB;
            }
        }
        tempB->father = r->father;
        r->father = tempB; //Keep this under the if/else

        tempA->UpdateHeight();
        r->UpdateHeight();
        tempB->UpdateHeight();

        if (r == this->root)
            this->root = tempB;

        UpdateRankedData_ByNode(tempA);
        UpdateRankedData_ByNode(r);
    }
    break;
    }
}

std::shared_ptr<Node> Avl::binarySearch(int insert_key, std::shared_ptr<Node> sub_root, INSERT_DIRECTION* dir)
{
    if (dir == nullptr)
    {
        //should'nt get in here...
        return nullptr;
    }

    if (sub_root.get() == nullptr)
    {
        return nullptr;
    }

    if (insert_key < sub_root->key)
    {
        if (sub_root->left == nullptr)
        {
            *dir = LEFT;
            return sub_root;
        }
        return binarySearch(insert_key, sub_root->left, dir);
    }
    else if (insert_key == sub_root->key)
    {
        return nullptr;
    }
    else
    {
        if (sub_root->right == nullptr)
        {
            *dir = RIGHT;
            return sub_root;
        }
        return binarySearch(insert_key, sub_root->right, dir);
    }
}

int Avl::GetMaxKey()
{
    return recGetMaxKey(this->root);
}

int Avl::recGetMaxKey(std::shared_ptr<Node> n)
{
    if (n == nullptr)
        return NULL;

    if (n->right == nullptr)
        return n->key;

    return recGetMaxKey(n->right);
}

std::shared_ptr<Level> Avl::getData(int key)
{
    std::shared_ptr<Node> n = findNode(this->root, key);
    if (n == nullptr)
        return NULL;

    return n->data;
}

std::shared_ptr<Node> Avl::findNode(std::shared_ptr<Node> sub_root, int key)
{
    if (sub_root == nullptr)
        return nullptr;

    if (key == sub_root->key)
    {
        return sub_root;
    }

    if (key < sub_root->key)
        return findNode(sub_root->left, key);

    return findNode(sub_root->right, key);
}

bool Avl::search(int key)
{
    std::shared_ptr<Node> n = findNode(root, key);
    return (n != nullptr);
}

void Avl::reverseInOrderFillArr(int arr[])
{
    int i = 0;
    reverseInOrderFillArr_rec(arr, root, &i);
}

void Avl::reverseInOrderFillArr_rec(int arr[], std::shared_ptr<Node> n, int* i)
{
    if (n == nullptr)
        return;

    reverseInOrderFillArr_rec(arr, n->right, i);

    arr[*i] = n->key;
    *i = (*i) + 1;

    reverseInOrderFillArr_rec(arr, n->left, i);
}

void Avl::InOrderFillArrData(std::shared_ptr<Level> arr[], int counter, bool fillArray)
{
    int i = 0;
    InOrderFillArrData_rec(arr, root, &i, counter, fillArray);
}

void Avl::InOrderFillArrData_rec(std::shared_ptr<Level> arr[], std::shared_ptr<Node> n, int* i, int count, bool fillArray)
{
    if (n == nullptr || *i >= count)
        return;

    InOrderFillArrData_rec(arr, n->left, i, count, fillArray);

    if (*i < count)
    {
        if (!fillArray)
            n->data = arr[*i];
        else
            arr[*i] = n->data;

        *i = (*i) + 1;
    }

    InOrderFillArrData_rec(arr, n->right, i, count, fillArray);
}

void Avl::InOrderFillArrKey(int arr[], int counter, bool fillArray)
{
    int i = 0;
    InOrderFillArrKey_rec(arr, root, &i, counter, fillArray);
}

void Avl::InOrderFillArrKey_rec(int arr[], std::shared_ptr<Node> n, int* i, int count, bool fillArray)
{
    if (n == nullptr || *i >= count)
        return;

    InOrderFillArrKey_rec(arr, n->left, i, count, fillArray);

    if (*i < count)
    {
        if (!fillArray)
            n->key = arr[*i];
        else
            arr[*i] = n->key;

        *i = (*i) + 1;
    }

    InOrderFillArrKey_rec(arr, n->right, i, count, fillArray);
}

void Avl::reverseInOrderRemoveNodes(int count)
{
    int i = 0;
    reverseInOrderRemoveNodes_rec(root, &i, count);
}

void Avl::reverseInOrderRemoveNodes_rec(std::shared_ptr<Node> n, int* i, int count)
{
    if (count == *i || n == nullptr)// || n->father == nullptr)
    {
        return;
    }

    reverseInOrderRemoveNodes_rec(n->right, i, count);

    if (n->height == 0) // leaf
    {
        if (n->father->left == n)
            n->father->left = nullptr;
        else
            n->father->right = nullptr;

        n->father->UpdateHeight();
        n->father = nullptr;

        *i = (*i) + 1;
    }

    reverseInOrderRemoveNodes_rec(n->left, i, count);
}

/* This function called ONLY when player is added\removed BUT the tree didnt changed (no new node added or removed) */
void Avl::UpdateRankedData(int startingPoint_Key)
{
    std::shared_ptr<Node> startNode = findNode(root, startingPoint_Key)->father;

    while(startNode != nullptr)
    {
        UpdateRankedData_Left (startNode);
        UpdateRankedData_Right(startNode);

        startNode = startNode->father;
    }
}

void Avl::UpdateRankedData_ByNode(std::shared_ptr<Node> startNode)
{
    while(startNode != nullptr)
    {
        UpdateRankedData_Left (startNode);
        UpdateRankedData_Right(startNode);

        startNode = startNode->father;
    }
}

void Avl::UpdateRankedData_Left(std::shared_ptr<Node> startNode)
{
    if (startNode->left != nullptr)
    {   
        startNode->min_left = startNode->left->min_left;
        startNode->num_players_left  = startNode->left->num_players_left + startNode->left->num_players_right +
                                        startNode->left->data->num_players;
        startNode->sum_left = startNode->left->sum_left + startNode->left->sum_right +
                                startNode->left->data->num_players * startNode->left->data->level;
        for(int i = 1; i < MAXSCALE ; i++)
        {
            startNode->left_hist[i] = startNode->left->left_hist[i] + startNode->left->right_hist[i] +
                                        startNode->left->data->hist[i];
        }
    }
}

void Avl::UpdateRankedData_Right(std::shared_ptr<Node> startNode)
{
    if(startNode->right != nullptr)
    { 
        startNode->max_right = startNode->right->max_right;

        startNode->num_players_right = startNode->right->num_players_left + startNode->right->num_players_right +
                                       startNode->right->data->num_players;
        
        startNode->sum_right = startNode->right->sum_left + startNode->right->sum_right +
                               startNode->right->data->num_players * startNode->right->data->level;

        for (int i = 1; i < MAXSCALE; i++)
        {
            startNode->right_hist[i] = startNode->right->left_hist[i] + startNode->right->right_hist[i] + 
                                       startNode->right->data->hist[i];
        }
    }
}