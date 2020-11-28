#include "priority_queue.h"

PriorityQueueResult pqClear(PriorityQueue queue){
    if(queue == NULL){
        return PriorityQueueResult PQ_NULL_ARGUMENT; 
    }
    PriorityQueue tempPQ = pqCopy(queue);
    while (tempPQ->Node->next){
        newPriorityQueue->Node->element = NULL;
        free(newPriorityQueue->Node->element);
        newPriorityQueue->Node->priority = NULL;
        free(newPriorityQueue->Node->priority);
        newPriorityQueue->Node->next = NULL;
        free(newPriorityQueue->Node->next);
        if(!pqRemove(tempPQ)){
            return PriorityQueueResult PQ_NULL_ARGUMENT;
        }
    }
    return PriorityQueueResult PQ_SUCCESS; 
}