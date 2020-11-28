#include "priority_queue.h"

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
