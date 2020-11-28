#include "priority_queue.h"

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