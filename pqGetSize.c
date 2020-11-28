#include "priority_queue.h"

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