/*************************************************************
* Author:        Philip Howard
* Email:         phil.howard@oit.edu
* Filename:      list.c
* Date Created:  2016-04-27
**************************************************************/

#include <stdlib.h>
#include <assert.h>

#include "list.h"

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
    assert(item != NULL);

    new_item->value = value;
    if (list->first == NULL || value < list->first->value) 
    {
        // special case inserting at beginning of list
        new_item->next = list->first;
        list->first = new_item;

        if (list->last == NULL) list->last = new_item;
    }
    else
    {
        item = list->first;
        while (item->next != NULL && item->next->value < value)
        {
            item = item->next;
        }

        //sched_yield();
        new_item->next = item->next;
        item->next = new_item;

        if (item->next == NULL) list->last = new_item;
    }

    list->count++;
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

    // insert at beginning
    new_item->next = list->first;
    list->first = new_item;

    if (list->last == NULL) list->last = new_item;

    list->count++;
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

    if (list->first == NULL) return 0;

    item = list->first;

    value = item->value;
    list->first = item->next;
    if (list->first == NULL) list->last = NULL;
    list->count--;

    free(item);

    return value;
}
