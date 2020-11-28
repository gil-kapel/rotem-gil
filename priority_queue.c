#include "priority_queue.h"
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

PriorityQueue_t {
    PriorityQueue_t *node{
        PQElement *element;
        PQElementPriority *priority;
        PriorityQueue_t *next;
    }Node;
    CopyPQElement copyElement;
    FreePQElement freeElement;
    EqualPQElements equalElements;
    CopyPQElementPriority copyPriority;
    FreePQElementPriority freePriority;
    ComparePQElementPriorities comparePriorities;
    PriorityQueue_t **iterator;
};


