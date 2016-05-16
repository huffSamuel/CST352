/*************************************************************
* Author:        Philip Howard
* Email:         phil.howard@oit.edu
* Filename:      list.c
* Date Created:  2016-04-27
* Modified:      Samuel Huff 5/1/2016
**************************************************************/

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "list_hand.h"

/********************************************************************** 
* Purpose: Initialize a list
*
* Precondition: 
*   None
*
* Postcondition: 
*   A pointer to a properly initialized list is returned
*
************************************************************************/
list_t *list_init()
{
    list_t *list = (list_t *)malloc(sizeof(list_t));
    assert(list != NULL);
    list->count = 0;
    list->first = NULL;
    list->last = NULL;

    pthread_mutex_init(&(list->lock), NULL);

    return list;
}

/********************************************************************** 
* Purpose: Add an item to the list in sorted order
*
* Precondition: 
*   list points to a well formed and sorted list
*
* Postcondition: 
*   A new item is added to the list in sorted order
*
************************************************************************/
void list_sorted_insert(list_t *list, int value)
{
    list_item_t *item;

    assert(list != NULL);

    list_item_t *new_item = (list_item_t *)malloc(sizeof(list_item_t));
    assert(new_item != NULL);

    new_item->value = value;
    new_item->next = NULL;
    pthread_mutex_init(&(new_item->lock), NULL);

    // Lock list
    pthread_mutex_lock(&(list->lock));
    if(list->count != 0)    // Lock first if needed
        pthread_mutex_lock(&(list->first->lock));
    
    // List locked
    // List first NOT locked
    if(list->count == 0)    // Insert into empty
    {
        pthread_mutex_lock(&(new_item->lock));
        list->first = new_item;
        list->last = new_item;
        pthread_mutex_unlock(&(list->lock));
        pthread_mutex_unlock(&(new_item->lock));
    }
    
    // List->first is locked
    // List is locked
    else if (value < list->first->value) // Insert at front
    {
        pthread_mutex_lock(&(new_item->lock));
        new_item->next = list->first;
        list->first = new_item;
        pthread_mutex_unlock(&(list->lock));
        pthread_mutex_unlock(&(new_item->next->lock));   // Unlocked the previous list->first
        pthread_mutex_unlock(&(list->first->lock));
    }

    // List->first is locked
    // List is locked
    else if(list->count == 1)   // Insert at end of 2 
    {
        pthread_mutex_lock(&(new_item->lock));
        list->first->next = new_item;
        pthread_mutex_unlock(&(list->first->lock)); // Unlock first
        list->last = new_item;
        pthread_mutex_unlock(&(new_item->lock));
        pthread_mutex_unlock(&(list->lock)); // Unlock list
    }

    // List->first is locked
    // List is locked
    else
    {
        // Should be good
        item = list->first;
        pthread_mutex_unlock(&(list->first->lock));
        pthread_mutex_lock(&(item->lock));
        pthread_mutex_unlock(&(list->lock));

        if(item->next != NULL) 
            pthread_mutex_lock(&(item->next->lock));
        //**********************************************************************
        while (item->next != NULL && item->next->value < value)
        {
            pthread_mutex_unlock(&(item->lock));
            item = item->next; 
            if(item->next != NULL)
                pthread_mutex_lock(&(item->next->lock));
        }
        // item, item->next

        pthread_mutex_lock(&(new_item->lock));
        new_item->next = item->next;
        item->next = new_item;
        pthread_mutex_unlock(&(item->lock));
       
        if (new_item->next == NULL) 
        {
            list->last = new_item;
            pthread_mutex_unlock(&(new_item->lock));
        }
        else
        {
            pthread_mutex_unlock(&(new_item->next->lock));
            pthread_mutex_unlock(&(new_item->lock));
        }
    }

    pthread_mutex_lock(&(list->lock));
    list->count++;
    pthread_mutex_unlock(&(list->lock));
}
/********************************************************************** 
* Purpose: Adds a new element to the front of the list
*
* Precondition: 
*   list points to a well formed list
*
* Postcondition: 
*   A new item is added to the front of the list
*
************************************************************************/
void list_push(list_t *list, int value)
{
    list_item_t *item;

    assert(list != NULL);

    list_item_t *new_item = (list_item_t *)malloc(sizeof(list_item_t));
    assert(item != NULL);

    new_item->value = value;
    new_item->next = NULL;
    pthread_mutex_init(&(new_item->lock), NULL);

    // insert at beginning
    new_item->next = list->first;
    list->first = new_item;

    if (list->last == NULL) list->last = new_item;


    list->count++;
    pthread_mutex_unlock(&(list->lock));
}
/********************************************************************** 
* Purpose: place an item at the end of the list
*
* Precondition: 
*   list points to a well formed list
*
* Postcondition: 
*   A new item is added to the end of the list
*
************************************************************************/
void list_push_end(list_t *list, int value)
{
    assert(list != NULL);

    list_item_t *item = (list_item_t *)malloc(sizeof(list_item_t));
    assert(item != NULL);

    item->value = value;
    item->next = NULL;

    pthread_mutex_lock(&(list->lock));
    if (list->last == NULL)
    {
        list->first = item;
        list->last = item;
    }
    else
    {
        list->last->next = item;
        list->last = item;
    }

    list->count++;
    pthread_mutex_unlock(&(list->lock));
}
/********************************************************************** 
* Purpose: Remove an item from the front of the list
*
* Precondition: 
*   list points to a well formed list
*
* Postcondition: 
*   The item at the front of the list is returned and removed.
*   If the list is empty, zero is returned
*
************************************************************************/
int list_pop(list_t *list)
{
    int value;
    list_item_t *item;

    pthread_mutex_lock(&(list->lock));
    if (list->first == NULL)
    { 
        return 0;
    }

    item = list->first;

    value = item->value;
    list->first = item->next;
    if (list->first == NULL) list->last = NULL;
    list->count--;
    pthread_mutex_unlock(&(list->lock));

    free(item);

    return value;
}
