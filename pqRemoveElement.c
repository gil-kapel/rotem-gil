#include "priority_queue.h"

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