#include "priority_queue.h"

PriorityQueue pqCreate(CopyPQElement copy_element, FreePQElement free_element, 
                       EqualPQElements equal_elements, CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority, ComparePQElementPriorities compare_priorities){
                           
                            if (!isElementValid()//need to determine
                            || !isPriorityValid()//need to determine
                            || something_else()){//need to determine
                                return PriorityQueueResult PQ_NULL_ARGUMENT;
                            }
                            PriorityQueue_t newPriorityQueue = malloc(sizeof(*newPriorityQueue));
                            if (newPriorityQueue == NULL){
                                return PriorityQueueResult PQ_NULL_ARGUMENT;
                            }
                            //* Insert NULL values to the node it self and create only the relevent functions to the priority queue */
                            newPriorityQueue->Node->element = NULL;
                            newPriorityQueue->Node->priority = NULL;
                            newPriorityQueue->Node->next = NULL;
                            newPriorityQueue->iterator = 0;
                            //* functions */
                            newPriorityQueue->copyElement = copy_element;
                            newPriorityQueue->freeElement = FreePQElement;
                            newPriorityQueue->equalElements = EqualPQElements;
                            newPriorityQueue->copyPriority = copy_element;
                            newPriorityQueue->freePriority = FreePQElement;
                            newPriorityQueue->comparePriorities = EqualPQElements;
                            return newPriorityQueue;
                       }
                       