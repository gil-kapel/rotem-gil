#include "priority_queue.h"
#define EMPTY 0

void pqDestroy(PriorityQueue queue){
    if(queue != NULL){
        pqClear(queue);
        free(queue);
    }
}