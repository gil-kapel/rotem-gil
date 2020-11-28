#include "priority_queue.h"

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
