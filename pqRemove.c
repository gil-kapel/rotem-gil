#include "priority_queue.h"

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