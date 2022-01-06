#ifndef UF_H_
#define UF_H_
#include <memory>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "library.h"
#include "avl.h"

template<typename E>
class UnionFind;
template<class E>
class UF_Node;

template<class E>
class UF_Node
{
    friend class UnionFind;
public:
    int count;
    std::shared_ptr<UF_Node<E>> next;
    std::shared_ptr<UF_Node<E>> head;
    E data;

    UF_Node(E& data) : count(0), data(data)
    {
        next = nullptr;
        head = nullptr;
    }

    ~UF_Node()
    {
        next = nullptr;
    }
    
};

/* ********************************** Union Find Class ********************************** */

template<class E>
class UnionFind
{
    friend class UF_Node;
public:
    UnionFind(int k) : size(k)
    {
        arr = new UF_Node<E>*[size];
    }
    ~UnionFind()
    {
        for (int i = 0; i < size; i++)
        {
            arr[i] = nullptr;
        }
        delete arr;
    }

    StatusType MakeSet(E& element, int id);
    E& Find(int id);
    E& Union(int g1, int g2);

private:
    int size;
    UF_Node<E>* arr[]; //Array of pointer to elemnts

};

template<class E>
StatusType UnionFind<E>::MakeSet(E& element, int id)
{
    if(id < 0 || id >= size)
        return INVALID_INPUT;

    arr[id] = new UF_Node<E>(element);
    if(arr[id] == nullptr)
        return ALLOCATION_ERROR;
    
    arr[id]->head = arr[id];
    arr[id]->count = 1;
    return SUCCESS;
}

template<class E>
E& UnionFind<E>::Find(int id)
{
    if(id < 0 || id >= size)
        return nullptr;
    return arr[id]->data;
}

template<class E>
E& UnionFind<E>::Union(int g1, int g2)
{
    if(g1 < 0 || g2 < 0 || g1 >= size || g2 >= size)
        return nullptr;

    UF_Node* a_head = arr[g1]->head;
    UF_Node* b_head = arr[g2]->head;
    if(a_head == b_head) //Can't union 2 elements from same group.
        return nullptr;
    

    UF_Node* temp, temp_next, head_new;
    int count1 = a_head->count;
    int count2 = b_head->count;
    if(count1 < count2)
    {
        head_new = b_head;
        a_head->next = head_new;
        head_new->count += count1;
        temp = arr[g1];
    }
    else
    {
        head_new = a_head;
        b_head->next = head_new;
        head_new->count += count2;
        temp = arr[g2];
    }

    temp_next = temp->next;
    while(temp != head_new)
    {
        temp->next = head_new;
        temp->head = head_new;
        temp = temp_next;
        temp_next = temp_next->next;
    }
    return head_new->data;
}

#endif