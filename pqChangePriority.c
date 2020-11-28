#include "priority_queue.h"

PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element, PQElementPriority old_priority, PQElementPriority new_priority){
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