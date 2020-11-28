#include "priority_queue.h"

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
}