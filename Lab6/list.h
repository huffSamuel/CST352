#pragma once
/*************************************************************
* Author:        Philip Howard
* Email:         phil.howard@oit.edu
* Filename:      list.h
* Date Created:  2016-04-26
* Modified:      Samuel Huff 5/1/2016
**************************************************************/
#include <pthread.h>
#include "queue.h"
//*******************************************************
// Struct for elements in a singly linked list
typedef struct list_item_s
{
    thread_t * value;                  // value stored in list element
    struct list_item_s *next;   // pointer to next list element
} list_item_t;

//*******************************************************
// struct for a list made up of list_item_t elements
typedef struct list_s
{
    int count;                  // number of items in the list
    struct list_item_s *first;  // pointer to first element in list
    struct list_item_s *last;   // pointer to last element in list
    pthread_mutex_t lock;       // Thread safe lock
} list_t;

//*******************************************************
// Initialize a list and return a pointer to it
list_t *list_init();

//*******************************************************
// Place an item into a list in sorted order
//
// list: pointer to the list
// value: value to be added to the list
void list_sorted_insert(list_t *list, thread_t * value);

//*******************************************************
// Add a new item to the beginning of a list
//
// list: pointer to the list
// value: value to be added to the list
void list_push(list_t *list, thread_t * value);

//*******************************************************
// Add a new item to the end of a list
//
// list: pointer to the list
// value: value to be added to the list
void list_push_end(list_t *list, thread_t * value);

//*******************************************************
// remove the first element in a list
//
// list: pointer to the list
// value: value to be added to the list
//
// Returns: the first item in the list, or zero if the list is empty
thread_t * list_pop(list_t *list);

int list_remove(list_t * list, thread_t * value);