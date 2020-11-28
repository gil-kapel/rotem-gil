#include "priority_queue.h"

PQElement pqGetFirst(PriorityQueue queue){
    if(queue == NULL || pqGetSize(queue)){
        return NULL; 
    }
    queue->iterator = NULL;
    return pqGetNext(queue);
