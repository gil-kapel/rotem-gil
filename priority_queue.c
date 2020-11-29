#include "priority_queue.h"
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

PriorityQueue_t mainPQ{
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

PriorityQueue pqCreate(CopyPQElement copy_element, FreePQElement free_element, 
                       EqualPQElements equal_elements, CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority, ComparePQElementPriorities compare_priorities){
                           
                            if (!isElementValid()//need to determine
                            || !isPriorityValid()//need to determine
                            || something_else()){//need to determine
                                return PriorityQueueResult PQ_NULL_ARGUMENT;
                            }
                            PriorityQueue_t newPriorityQueue = malloc(sizeof(*newPriorityQueue));
                            if (newPriorityQueue == NULL){
                                return PriorityQueueResult PQ_NULL_ARGUMENT;
                            }
                            //* Insert NULL values to the node it self and create only the relevent functions to the priority queue */
                            newPriorityQueue->Node->element = NULL;
                            newPriorityQueue->Node->priority = NULL;
                            newPriorityQueue->Node->next = NULL;
                            newPriorityQueue->iterator = 0;
                            //* functions */
                            newPriorityQueue->copyElement = copy_element;
                            newPriorityQueue->freeElement = FreePQElement;
                            newPriorityQueue->equalElements = EqualPQElements;
                            newPriorityQueue->copyPriority = copy_element;
                            newPriorityQueue->freePriority = FreePQElement;
                            newPriorityQueue->comparePriorities = EqualPQElements;
                            return newPriorityQueue;
                       }


void pqDestroy(PriorityQueue queue){
    if(queue != NULL){
        pqClear(queue);
        free(queue);
    }
}

PriorityQueue pqCopy(PriorityQueue queue){
    if(queue == NULL){
        return PriorityQueueResult PQ_NULL_ARGUMENT; 
    }
    PriorityQueue_t newPriorityQueue = pqCreate(queue->copyElement, queue->freeElement, queue->equalElements,
                                                queue->copyPriority, queue->freePriority, queue->comparePriorities);
    *newPriorityQueue = *queue;
    newPriorityQueue->iterator = queue->iterator;
    return newPriorityQueue;
}

int pqGetSize(PriorityQueue queue){
    if (queue == NULL){
        retrun -1;
    }
    int counter = 0;
    PriorityQueue tempPQ = pqCopy(queue);
    while (tempPQ->next){
        counter++;
        if(!pqRemove(tempPQ)){
            return -1;
        }
    }
    return counter;
}

bool pqContains(PriorityQueue queue, PQElement element){
    if (queue==NULL || element==NULL){
        return FALSE;
    }
    PriorityQueue tempPQ = pqCopy(queue);
    while (tempPQ->Node->next){
        if (equalElements(queue->Node->element, element){
            return TRUE;
        }
        else if(!pqRemove(tempPQ)){
            return FALSE;
        }
        else continue;
    }
    return FALSE
}

PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority){
    if(queue == NULL){
        return PriorityQueueResult PQ_NULL_ARGUMENT; 
    }
    queue->Node->element = queue->copyElement(element);
    if(queue->Node->element==NULL){
        return PriorityQueueResult PQ_OUT_OF_MEMORY; 
    }
    queue->Node->priority = queue->copyPriority(priority);
    if(queue->Node->priority==NULL){
        return PriorityQueueResult PQ_OUT_OF_MEMORY; 
    }
    return PriorityQueueResult PQ_SUCCESS;
}

PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element,
                                     PQElementPriority old_priority, PQElementPriority new_priority){
    if(queue == NULL){
        return PriorityQueueResult PQ_NULL_ARGUMENT; 
    }
    while(queue->comparePriorities(queue->Node->priority, old_priority)){
        if(queue->Node->next==NULL){
            return PriorityQueueResult PQ_ELEMENT_DOES_NOT_EXISTS;
        }
        queue = queue->Node->next;
    }
    while(queue->equalElements(element, queue->Node->element)){
        if(queue->Node->priority==NULL){
            return PriorityQueueResult PQ_OUT_OF_MEMORY;
        }
        queue = queue->Node->next;
    }
    queue->Node->priority = copyPriority(new_priority);
    return PriorityQueueResult PQ_SUCCESS;
}

PriorityQueueResult pqRemove(PriorityQueue queue){
    if(queue == NULL){
        return PriorityQueueResult PQ_NULL_ARGUMENT; 
    }
    PriorityQueue tempPQ = pqCopy(queue);
    do{
        queue = tempPQ->Node->next;
        pqDestroy(tempPQ);
    } while (!queue->comparePriorities(tempPQ->Node->priority, queue->Node->priority));
    queue->iterator = NULL;
    return PriorityQueueResult PQ_SUCCESS; 
}

PriorityQueueResult pqRemoveElement(PriorityQueue queue, PQElement element){
    if(queue == NULL){
        return PriorityQueueResult PQ_NULL_ARGUMENT; 
    }
    while(queue->equalElements(element, queue->Node->element)){
        if(queue->Node->next==NULL){
            return PriorityQueueResult PQ_ELEMENT_DOES_NOT_EXISTS;
        }
        queue = queue->Node->next;
    }
    void* elementPriority = queue->Node->priority;
    while(queue->comparePriorities(queue->Node->priority,elementPriority)){
        queue->freeElement(queue->Node->element);
        queue->freePriority(queue->Node->priority);
        queue = queue->Node->next;
    }
    return PriorityQueueResult PQ_SUCCESS;
}

PQElement pqGetNext(PriorityQueue queue){
    if(queue == NULL){
        return PriorityQueueResult PQ_NULL_ARGUMENT; 
    }
    if (queue->iterator >= pqGetSize(queue)) {
        return NULL;
    }
    queue = queue->Node->next;
    queue->iterator = *queue;
    return queue->Node->element;
}

PQElement pqGetFirst(PriorityQueue queue){
    if(queue == NULL || pqGetSize(queue)){
        return NULL; 
    }
    queue->iterator = NULL;
    return pqGetNext(queue);
}

PriorityQueueResult pqClear(PriorityQueue queue){
    if(queue == NULL){
        return PriorityQueueResult PQ_NULL_ARGUMENT; 
    }
    PriorityQueue tempPQ = pqCopy(queue);
    while (tempPQ->Node->next){
        newPriorityQueue->Node->element = NULL;
        queue->freeElement(newPriorityQueue->Node->element);
        newPriorityQueue->Node->priority = NULL;
        queue->freePriority(newPriorityQueue->Node->priority);
        newPriorityQueue->Node->next = NULL;
        free(newPriorityQueue->Node->next);
        if(!pqRemove(tempPQ)){
            return PriorityQueueResult PQ_NULL_ARGUMENT;
        }
    }
    return PriorityQueueResult PQ_SUCCESS; 
}