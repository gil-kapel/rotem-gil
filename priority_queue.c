#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>

#define INVALID -1
#define EMPTY 0
#define EQUAL 0
/*!
* Macro for reseting the iterator of the queue
*/ 
#define RESET_ITERATOR(queue) queue->iterator = NULL;

/*!
* Macro for testing if a copy function succeed
* free the node if failed
* destroy the queue if the failure was when copying a new queue
*/ 
#define PQ_COPY_DATA_TEST(queue, node, data_type) \
    (node->data_type == NULL)\
    {\
        free(node);\
        free(queue);
/*!
* Macro for testing if a NULL pointer was sent
* or if a malloc failed
*/ 
#define PQ_NULL_TEST(data_type, result)\
    if (data_type == NULL)\
    {\
        return result;\
    }

typedef struct node_t
{
    PQElement element;
    PQElementPriority priority;
    struct node_t *next;
}*Node;

struct PriorityQueue_t
{
    Node head;
    Node iterator;
    CopyPQElement copyElement;
    FreePQElement freeElement;
    EqualPQElements equalElements;
    CopyPQElementPriority copyPriority;
    FreePQElementPriority freePriority;
    ComparePQElementPriorities comparePriorities;
};

static Node pqNodeCreate()
{
    Node newNode = malloc(sizeof(*newNode));
    PQ_NULL_TEST(newNode, NULL)
    newNode->element = NULL;
    newNode->priority = NULL;
    newNode->next = NULL;
    return newNode;
}

//* insert a new node to the queue, insert the new node after the 'position' node or at the top of the queue
static PriorityQueueResult pqInsertNewNode(PriorityQueue queue, PQElement element,
                                           PQElementPriority priority, Node position)
{
    PQ_NULL_TEST(queue, PQ_OUT_OF_MEMORY)
    Node newNode = pqNodeCreate();
    newNode->element = queue->copyElement(element);
    if PQ_COPY_DATA_TEST(NULL, newNode, element)
        return PQ_OUT_OF_MEMORY;
    }
    newNode->priority = queue->copyPriority(priority);
    if PQ_COPY_DATA_TEST(NULL, newNode, priority)
        return PQ_OUT_OF_MEMORY;
    }
    //* there are 4 possibilites to add a newNode:
    //* queue is empty -- 1) add to a free space
    //* queue is full -- 2)add to the buttom, 3) add to the middle, 4) add to the top
    if(queue->head == NULL) //an empty pq - add the first node to the queue
    {
        queue->head = newNode;
    }
    else if (position == NULL) //*the queue isn't empty, insert the new node to the buttom of the queue
    {
        position->next = newNode; //*newNode->next is NULL already as created
    }
    else if (position == queue->head) //*add the newNode to the top of the queue
    {
        newNode->next = position;
        queue->head = newNode;
    }
    else //* add the newNode to the middle of the queue
    {
        newNode->next = position->next;
        position->next = newNode;
    }


    return PQ_SUCCESS;
} 

PriorityQueue pqCreate(CopyPQElement copy_element, FreePQElement free_element, 
                       EqualPQElements equal_elements, CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority, ComparePQElementPriorities compare_priorities)
{
    PriorityQueue newPriorityQueue = malloc(sizeof(*newPriorityQueue));
    PQ_NULL_TEST(newPriorityQueue, NULL)
    newPriorityQueue->head              = NULL;
    newPriorityQueue->iterator          = NULL;
    //* functions */
    newPriorityQueue->copyElement       = copy_element;
    newPriorityQueue->freeElement       = free_element;
    newPriorityQueue->equalElements     = equal_elements;
    newPriorityQueue->copyPriority      = copy_priority;
    newPriorityQueue->freePriority      = free_priority;
    newPriorityQueue->comparePriorities = compare_priorities;
    return newPriorityQueue;
}

PriorityQueueResult pqClear(PriorityQueue queue)
{
    PQ_NULL_TEST(queue, PQ_NULL_ARGUMENT)
    
    if(queue->head==NULL) //*the queue is empty
    {
        RESET_ITERATOR(queue);
        return PQ_SUCCESS; 
    }
    Node head = queue->head;
    while (head)
    //free the element and priority of the node/
    //copy the next node address and free the node itself
    //- while loop will stop when there are no next node 
    {
        queue->freeElement(head->element);
        queue->freePriority(head->priority);
        Node tail = head;
        head = head->next;
        free(tail);
    }
    RESET_ITERATOR(queue);;
    return PQ_SUCCESS; 

}

void pqDestroy(PriorityQueue queue)
{
    if(queue != NULL)
    {
        pqClear(queue);
        free(queue);
    }
}

PriorityQueue pqCopy(PriorityQueue queue)
{
    PQ_NULL_TEST(queue, NULL)
    PriorityQueue newPriorityQueue = pqCreate(queue->copyElement, queue->freeElement, queue->equalElements,
                                                queue->copyPriority, queue->freePriority, queue->comparePriorities);
    if(queue->head == NULL) //* copy an empty queue
    {  
        RESET_ITERATOR(queue);
        return newPriorityQueue;
    }
    Node headNode = pqNodeCreate(); //*create a new node and copy the head of the origin queue to it
    headNode->element = queue->copyElement(queue->head->element);
    if PQ_COPY_DATA_TEST(newPriorityQueue, headNode, element)
        RESET_ITERATOR(queue);
        return NULL;
    }
    headNode->priority = queue->copyPriority(queue->head->priority);
    if PQ_COPY_DATA_TEST(newPriorityQueue, headNode, priority)
        RESET_ITERATOR(queue);
        return NULL;
    }
    newPriorityQueue->head = headNode; //* save the head of the new queue
    Node tail = queue->head->next;
    while(tail) //*copy the queue from the second node / if the queue has only one node, it will be copied and won't get in the while loop
    {
        if(pqInsert(newPriorityQueue,tail->element,tail->priority) != PQ_SUCCESS) //* the pqInsert function will care that the nodes will enter in the right order
        {
            pqDestroy(newPriorityQueue); // if an element or priority copy won't succeed the new queue must be destroyed
            RESET_ITERATOR(queue);
            return NULL;
        }
        tail = tail->next;
    }
    RESET_ITERATOR(queue);
    RESET_ITERATOR(newPriorityQueue);
    return newPriorityQueue;
}

int pqGetSize(PriorityQueue queue)
{
    PQ_NULL_TEST(queue, INVALID)
    int counter = EMPTY;
    Node head = queue->head;
    while (head) //start with the first node because when we create a new pq - head = NULL
    {
        counter++;
        head = head->next;
    }
    return counter;
}

bool pqContains(PriorityQueue queue, PQElement element)
{
    PQ_NULL_TEST(queue, false)
    PQ_NULL_TEST(element, false)
    Node head = queue->head;
    while (head)
    {
        if (queue->equalElements(queue->head->element, element))
        {
            return true;
        }
        head = head->next;
    }
    return false;
}

PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
    PQ_NULL_TEST(queue, PQ_NULL_ARGUMENT)
    if(element == NULL)
    {
        RESET_ITERATOR(queue)
        return PQ_NULL_ARGUMENT;
    }
    if(priority == NULL)
    {
        RESET_ITERATOR(queue)
        return PQ_NULL_ARGUMENT;
    }
    Node position = queue->head;
    if (queue->head==NULL) //* the queue is empty
    {
        RESET_ITERATOR(queue);
        return pqInsertNewNode(queue,element,priority,NULL); //add the new node to the head of the queue
    }
    while(position)
    {
        if(queue->comparePriorities(position->priority, priority) <= EQUAL)
        //* if the new element has the highest priority, the new node will be insert to/
        //  the top and become the new head of the queue
        //  else the new node will insert after the position pointer *
        {
            RESET_ITERATOR(queue);
            return pqInsertNewNode(queue,element,priority,position);
        }
        position = position->next;
    }
    return PQ_SUCCESS;
}

PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element,
                                     PQElementPriority old_priority, PQElementPriority new_priority)
{
    PQ_NULL_TEST(queue, PQ_NULL_ARGUMENT)
    if(element == NULL)
    {
        RESET_ITERATOR(queue)
        return PQ_NULL_ARGUMENT;
    }
    if(old_priority == NULL)
    {
        RESET_ITERATOR(queue)
        return PQ_NULL_ARGUMENT;
    }
    if(new_priority == NULL)
    {
        RESET_ITERATOR(queue)
        return PQ_NULL_ARGUMENT;
    }
    Node head = queue->head;
    while(head)
    {
        if(queue->comparePriorities(head->priority, old_priority) < EQUAL)
        //* itarate the queue until reaches to the old priority
        {
            head = head->next;
            continue;
        }
        else if(queue->comparePriorities(head->priority, old_priority) == EQUAL)
        //**will replace the first node with the old priority
        {
            if(queue->equalElements(element, head->element)) //**will replace only an identical element
            {
                PriorityQueueResult removeResult = pqRemoveElement(queue, head->element);
                //* remove the element and insert a new one
                if(removeResult != PQ_SUCCESS) //* if the pqRemoveElement function didn't work
                {
                    return removeResult;
                }
                return pqInsert(queue, element, new_priority); //* insert the new node with the new priority
            }
            head = head->next;
            continue;
        }
        RESET_ITERATOR(queue);
        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }
    RESET_ITERATOR(queue);
    return PQ_ELEMENT_DOES_NOT_EXISTS;
}

PriorityQueueResult pqRemove(PriorityQueue queue)
{
    PQ_NULL_TEST(queue, PQ_NULL_ARGUMENT)
    queue->freeElement(queue->head->element);
    queue->freePriority(queue->head->priority);
    Node toDelete = queue->head;
    queue->head = queue->head->next;
    free(toDelete);
    RESET_ITERATOR(queue)
    return PQ_SUCCESS; 
}

PriorityQueueResult pqRemoveElement(PriorityQueue queue, PQElement element)
{
    PQ_NULL_TEST(queue, PQ_NULL_ARGUMENT)
    if(queue->head == NULL)
    //* empty queue
    {
        RESET_ITERATOR(queue)
        return PQ_SUCCESS;
    }
    if(queue->equalElements(queue->head->element, element) == true)
    //* the first element of the queue needs to be removed
    {
        return pqRemove(queue);
    }
    if(queue->head->next == NULL)
    //* the queue contains one node that don't have the needed element
    {
        RESET_ITERATOR(queue)
        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }
    Node position = queue->head;
    while(position)
    {
        if(queue->equalElements(position->next->element, element) == true)
        //*compare the next element, save the previus node to patch the queue after the delete
        {
            queue->freeElement(position->next->element);
            queue->freePriority(position->next->priority);
            Node toDelete = position->next;
            position->next = position->next->next;
            //*before removing the node, we change the next pointer of the previous node
            free(toDelete);
            RESET_ITERATOR(queue)
            return PQ_SUCCESS; 

        }
        position = position->next;
    }
    RESET_ITERATOR(queue);
    return PQ_ELEMENT_DOES_NOT_EXISTS; 
}

PQElement pqGetFirst(PriorityQueue queue)
{
    PQ_NULL_TEST(queue, NULL)
    if(queue->head == NULL)
    {
        RESET_ITERATOR(queue);;
        return NULL;
    }
    queue->iterator = queue->head;
    return queue->head->element;
}

PQElement pqGetNext(PriorityQueue queue)
{
    PQ_NULL_TEST(queue, NULL)
    PQ_NULL_TEST(queue->iterator, NULL)
    if(queue->iterator->next == NULL)
    {
        RESET_ITERATOR(queue);
        return NULL;
    }
    queue->iterator = queue->iterator->next;
    return queue->iterator->element;
}
