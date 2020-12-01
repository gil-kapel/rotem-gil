#include "priority_queue.h"
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#define FALSE 0
#define TRUE 1

typedef struct node_t{
        PQElement element;
        PQElementPriority priority;
        struct node_t *next;
}*Node;

struct PriorityQueue_t{
    Node node;
    CopyPQElement copyElement;
    FreePQElement freeElement;
    EqualPQElements equalElements;
    CopyPQElementPriority copyPriority;
    FreePQElementPriority freePriority;
    ComparePQElementPriorities comparePriorities;
};

static void nextNode(Node node){
        Node temp = node; 
        node->next = temp; 
}

PriorityQueue pqCreate(CopyPQElement copy_element, FreePQElement free_element, 
                       EqualPQElements equal_elements, CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority, ComparePQElementPriorities compare_priorities){
                            PriorityQueue newPriorityQueue = malloc(sizeof(newPriorityQueue));
                            if (newPriorityQueue == NULL){
                                return NULL;
                            }
                            Node newNode = malloc(sizeof(newNode));
                            if (newNode == NULL){ 
                                return NULL;
                            }
                            newPriorityQueue->node = newNode;
                            //* Insert NULL values to the node it self and create only the relevent functions to the priority queue */
                            newPriorityQueue->node->element = 0;
                            newPriorityQueue->node->priority = 0;
                            newPriorityQueue->node->next = NULL;
                            //* functions */
                            newPriorityQueue->copyElement = copy_element;
                            newPriorityQueue->freeElement = free_element;
                            newPriorityQueue->equalElements = equal_elements;
                            newPriorityQueue->copyPriority = copy_element;
                            newPriorityQueue->freePriority = free_priority;
                            newPriorityQueue->comparePriorities = compare_priorities;
                            return newPriorityQueue;
                       }


PriorityQueueResult pqClear(PriorityQueue queue){
    if(queue == NULL){
        return PQ_NULL_ARGUMENT; 
    }
    while (queue->node){
        Node toDelete = malloc(sizeof(Node));
        if (toDelete == NULL){
            return PQ_NULL_ARGUMENT;
        }
        toDelete = queue->node;
        queue->node = queue->node->next;   
        free(toDelete);
    }
    return PQ_SUCCESS; 
}

void pqDestroy(PriorityQueue queue){
    if(queue != NULL){
        pqClear(queue);
        free(queue->node);
        free(queue);
    }
}

PriorityQueue pqCopy(PriorityQueue queue){
    if(queue == NULL){
        return NULL; 
    }
    PriorityQueue newPriorityQueue = pqCreate(queue->copyElement, queue->freeElement, queue->equalElements,
                                                queue->copyPriority, queue->freePriority, queue->comparePriorities);
    *newPriorityQueue = *queue;
    return newPriorityQueue;
}

int pqGetSize(PriorityQueue queue){
    if (queue == NULL){
        return -1;
    }
    int counter = 0;
    PriorityQueue tempPQ = pqCopy(queue);
    while (tempPQ->node){
        counter++;
        Node toDelete = tempPQ->node;
        tempPQ->node = tempPQ->node->next;   
        free(toDelete);
    }
    return counter;
}

bool pqContains(PriorityQueue queue, PQElement element){
    if (queue==NULL || element==NULL){
        return FALSE;
    }
    PriorityQueue tempPQ = pqCopy(queue);
    while (tempPQ->node->next){
        if (queue->equalElements(queue->node->element, element)){
            return TRUE;
        }
        else if(!pqRemove(tempPQ)){
            return FALSE;
        }
        else continue;
    }
    return FALSE;
}

PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority){
    if(queue == NULL){
        return PQ_NULL_ARGUMENT; 
    }
    queue->node->element = queue->copyElement(element);
    if(queue->node->element==NULL){
        return PQ_OUT_OF_MEMORY; 
    }
    queue->node->priority = queue->copyPriority(priority);
    if(queue->node->priority==NULL){
        return PQ_OUT_OF_MEMORY; 
    }
    return PQ_SUCCESS;
}

PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element,
                                     PQElementPriority old_priority, PQElementPriority new_priority){
    if(queue == NULL){
        return PQ_NULL_ARGUMENT; 
    }
    while(queue->comparePriorities(queue->node->priority, old_priority)){
        if(queue->node->next==NULL){
            return PQ_ELEMENT_DOES_NOT_EXISTS;
        }
        nextNode(queue->node);
    }
    while(queue->equalElements(element, queue->node->element)){
        if(queue->node->priority==NULL){
            return PQ_OUT_OF_MEMORY;
        }
        nextNode(queue->node);
    }
    queue->node->priority = queue->copyPriority(new_priority);
    return PQ_SUCCESS;
}

PriorityQueueResult pqRemove(PriorityQueue queue){
    if(queue == NULL){
        return PQ_NULL_ARGUMENT; 
    }
    PriorityQueue tempPQ = pqCopy(queue);
    do{
        nextNode(queue->node);
    } while (!queue->comparePriorities(tempPQ->node->priority, queue->node->priority));
    return PQ_SUCCESS; 
}

PriorityQueueResult pqRemoveElement(PriorityQueue queue, PQElement element){
    if(queue == NULL){
        return PQ_NULL_ARGUMENT; 
    }
    while(queue->equalElements(element, queue->node->element)){
        if(queue->node->next==NULL){
            return PQ_ELEMENT_DOES_NOT_EXISTS;
        }
        nextNode(queue->node);
    }
    void* elementPriority = queue->node->priority;
    while(queue->comparePriorities(queue->node->priority,elementPriority)){
        queue->freeElement(queue->node->element);
        queue->freePriority(queue->node->priority);
        nextNode(queue->node);
    }
    return PQ_SUCCESS;
}

PQElement pqGetNext(PriorityQueue queue){
    if(queue == NULL){
        return NULL; 
    }
    nextNode(queue->node);
    return queue->node->element;
}

PQElement pqGetFirst(PriorityQueue queue){
    if(queue == NULL || pqGetSize(queue)){
        return NULL; 
    }
    return pqGetNext(queue);
}

