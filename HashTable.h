#ifndef HT_H_
#define HT_H_
#include <memory>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "library2.h"
#define INITIAL_ARR_LEN 2

template<typename E>
class HashTable;
template<class E>
class HT_Node;

template<class E>
class HT_Node
{
    friend class HashTable<E>;
public:
    std::shared_ptr<HT_Node<E>> next;
    std::shared_ptr<HT_Node<E>> prev;
    E data; //will hold the element data.
    int key;

    HT_Node(E data, int key) : data(data), key(key)
    {
        next = nullptr;
        prev = nullptr;
    }

    ~HT_Node()
    {
        next = nullptr;
        prev = nullptr;
    }

};

/* ********************************** Hash Table Class ********************************** */

template<class E>
class HashTable
{
    friend class HT_Node<E>;
public:
    HashTable() : currentArrSize(INITIAL_ARR_LEN), currentCount(0) //default Constructor.
    {
        arr = new std::shared_ptr<HT_Node<E>>[currentArrSize];
        for (int i = 0; i < currentArrSize; i++)
        {
            arr[i] = nullptr;
        }
    }

    HashTable(int size) : currentArrSize(size), currentCount(0) //Constructor.
    {
        //make sure size is 2^k of some k...
        if (size % 2 == 0)
        {
            arr = new std::shared_ptr<HT_Node<E>>[size];
            for (int i = 0; i < currentArrSize; i++)
            {
                arr[i] = nullptr;
            }
        }
    }

    ~HashTable()
    {
        for (int i = 0; i < currentArrSize; i++)
        {
            arr[i] = nullptr;
        }
        delete[] arr;
    }

    StatusType Insert(E data, int key);
    E Find(int key, StatusType* st);
    StatusType Remove(int key);

    int GetSize_DEBUG() {
        return this->currentArrSize;
    }

    int GetCount_DEBUG() {
        return this->currentCount;
    }

private:
    int currentArrSize; //hold the current size of the array. Initial value is 2.
    int currentCount;   //holds the amount of elements in the array. when this gets to ArrSize/2  ---> need to multilply(2X) the arr size.
    
    std::shared_ptr<HT_Node<E>>* arr; //Array of pointers to elements.


    int HashFunc(int key);
    StatusType Insert_Helper(std::shared_ptr<HT_Node<E>> Element, int key, std::shared_ptr<HT_Node<E>> array[]);
    StatusType MultiplyOrDivide_Arr();
};

template<class E>
StatusType HashTable<E>::Remove(int key)
{
    if (arr[HashFunc(key)] == nullptr)
        return FAILURE;
    StatusType res = FAILURE;
    std::shared_ptr<HT_Node<E>> ptr = arr[HashFunc(key)];

    while (ptr != nullptr)
    {
        if (ptr->key == key)
        {
            if (ptr->prev == nullptr)
            {
                arr[HashFunc(key)] = ptr->next;
                if (ptr->next != nullptr)
                    ptr->next->prev = nullptr;
                ptr->next = nullptr;
            }
            else
            {
                ptr->prev->next = ptr->next;
                if (ptr->next != nullptr)
                    ptr->next->prev = ptr->prev;
                ptr->next = nullptr;
                ptr->prev = nullptr;
            }
            res = SUCCESS;
            break;
        }
        ptr = ptr->next;
    }
    if (res != SUCCESS)
        return FAILURE;

    currentCount--;
    return MultiplyOrDivide_Arr();
}

template<class E>
int HashTable<E>::HashFunc(int key)
{
    return (key % currentArrSize);
}

template<class E>
StatusType HashTable<E>::Insert(E data, int key)
{
    std::shared_ptr<HT_Node<E>> newElement = std::shared_ptr<HT_Node<E>>(new HT_Node<E>(data, key));
    if (newElement == nullptr)
        return ALLOCATION_ERROR;


    StatusType res = Insert_Helper(newElement, key, arr);
    if (res == FAILURE)
        return res;

    currentCount++;
    return MultiplyOrDivide_Arr();
}

template<class E>
StatusType HashTable<E>::Insert_Helper(std::shared_ptr<HT_Node<E>> Element, int key, std::shared_ptr<HT_Node<E>> array[])
{
    if (array[HashFunc(key)] == nullptr)
    {
        array[HashFunc(key)] = Element;
    }
    else //insert to the chain...
    {
        //1. run throuh the list to check if found there. if found = return FAILURE.
        //2. if not found, add to the head of the list.
        std::shared_ptr<HT_Node<E>> ptr = array[HashFunc(key)];
        while (ptr != nullptr)
        {
            if (ptr->key == key)
                return FAILURE;
            ptr = ptr->next;
        }
        ptr = array[HashFunc(key)];
        array[HashFunc(key)] = Element;
        Element->next = ptr;
        ptr->prev = Element;
    }
    return SUCCESS;
}

template<class E>
StatusType HashTable<E>::MultiplyOrDivide_Arr()
{
    if (currentCount > (int)(currentArrSize / 4.0f) && currentCount < (int)(currentArrSize / 2.0f))
        return SUCCESS;
    if (currentCount == 0) //This will applt only when REMOVING the last element ! (make sure the length is atleast 2)
        return SUCCESS;

    double multFactor = (currentCount >= (int)(currentArrSize / 2.0f)) ? 2 : 0.5f;

    std::shared_ptr<HT_Node<E>>* temp;
    temp = new std::shared_ptr<HT_Node<E>>[(int)(currentArrSize * multFactor)];
    if (temp == nullptr)
        return ALLOCATION_ERROR;

    for (int i = 0; i < (int)(currentArrSize * multFactor); i++)
    {
        temp[i] = nullptr;
    }

    int oldSize = currentArrSize;
    currentArrSize *= multFactor;

    for (int i = 0; i < oldSize; i++)
    {
        std::shared_ptr<HT_Node<E>> ptr = arr[i];
        std::shared_ptr<HT_Node<E>> ptr_temp = arr[i];
        while (ptr != nullptr) // the chain is not empty.
        {
            ptr_temp = ptr->next;
            StatusType res = Insert_Helper(ptr, ptr->key, temp);
            if (res != SUCCESS)
            {
                delete[] temp;
                return res;
            }
            ptr = ptr_temp;
            if (ptr_temp != nullptr)
                ptr_temp->prev = nullptr;
        }
    }

    for (int i = 0; i < oldSize; i++)
    {
        arr[i] = nullptr;
    }
    delete[] arr;
    arr = temp;

    return SUCCESS;
}

template<class E>
E HashTable<E>::Find(int key, StatusType* st)
{
    if (arr[HashFunc(key)] == nullptr)
    {
        *st = FAILURE;
        return E();
    }
    else //search in the chain.
    {
        std::shared_ptr<HT_Node<E>> ptr = arr[HashFunc(key)];
        while (ptr != nullptr)
        {
            if (ptr->key == key)
            {
                *st = SUCCESS;
                return ptr->data;
            }
            ptr = ptr->next;
        }
    }
    *st = FAILURE;
    return E();
}

#endif