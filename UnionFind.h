#ifndef UF_H_
#define UF_H_
#include <memory>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "library2.h"
#include "avl.h"

template<typename E>
class UnionFind;
template<class E>
class UF_Node;

template<class E>
class UF_Node
{
    friend class UnionFind<E>;
public:
    int count;
    std::shared_ptr<UF_Node<E>> next;
    std::shared_ptr<UF_Node<E>> head;
    E data; //will hold the group data.

    UF_Node(E data) : count(0), data(data)
    {
        next = nullptr;
        head = nullptr;
    }

    ~UF_Node()
    {
        next = nullptr;
        head = nullptr;
    }

};

/* ********************************** Union Find Class ********************************** */

template<class E>
class UnionFind
{
    friend class UF_Node<E>;
public:
    UnionFind(int k) : size(k)
    {
        if (k >= 0)
        {
            arr = new std::shared_ptr<UF_Node<E>>[k];
        }
    }
    ~UnionFind()
    {
        for (int i = 0; i < size; i++)
        {
            if (arr[i] == nullptr)
                continue;
            arr[i]->next = nullptr;
            arr[i]->head = nullptr;
            arr[i] = nullptr;
        }
        delete[] arr;
    }

    StatusType MakeSet(E element, int id);
    E Find(int id, StatusType* st);
    E Union(int g1, int g2, StatusType* st);

private:
    int size;
    std::shared_ptr<UF_Node<E>>* arr; //Array of pointer to elements

};

template<class E>
StatusType UnionFind<E>::MakeSet(E element, int id)
{
    if (id < 0 || id >= size)
        return INVALID_INPUT;

    arr[id] = std::shared_ptr<UF_Node<E>> (new UF_Node<E>(element));
    if (arr[id] == nullptr)
        return ALLOCATION_ERROR;

    arr[id]->head = arr[id];
    arr[id]->count = 1;
    return SUCCESS;
}

template<class E>
E UnionFind<E>::Find(int id, StatusType* st)
{
    if (id < 0 || id >= size)
    {
        *st = INVALID_INPUT;
        return E();
    }
    *st = SUCCESS;
    return arr[id]->head->data;
}

template<class E>
E UnionFind<E>::Union(int g1, int g2, StatusType* st)
{
    if (g1 <= 0 || g2 <= 0 || g1 >= size || g2 >= size)
    {
        *st = INVALID_INPUT;
        return E();
    }
    std::shared_ptr<UF_Node<E>> a_head = arr[g1]->head;
    std::shared_ptr<UF_Node<E>> b_head = arr[g2]->head;
    if (a_head == b_head) //Can't union 2 elements from same group - return SUCCES !
    {
        *st = SUCCESS;
        return E();
    }

    std::shared_ptr<UF_Node<E>> temp;
    std::shared_ptr<UF_Node<E>> temp_next;
    std::shared_ptr<UF_Node<E>> head_new;
    int count1 = a_head->count;
    int count2 = b_head->count;
    if (count1 < count2)
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
    while (temp != head_new)
    {
        temp->next = head_new;
        temp->head = head_new;
        temp = temp_next;
        temp_next = temp_next->next;
    }
    *st = SUCCESS;
    return head_new->data;
}

#endif