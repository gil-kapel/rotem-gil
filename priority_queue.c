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
* Macro for testing if a NULL pointer was sent
* or if a malloc failed
*/ 
#define PQ_NULL_TEST(ptr, result)\
    if (ptr == NULL)\
    {\
        return result;\
    }
/*!
* Macro for testing if a NULL data was sent
*/
#define PQ_NULL_DATA_TEST(queue,data_type, result)\
    if (data_type == NULL)\
    {\
        RESET_ITERATOR(queue)\
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

static Node pqNodeCreate(PriorityQueue queue, PQElement element,
                                           PQElementPriority priority)
{
    Node newNode = malloc(sizeof(*newNode));
    PQ_NULL_TEST(newNode, NULL)
    newNode->next = NULL;
    newNode->element = queue->copyElement(element);
    if(newNode->element == NULL)
    {
        free(newNode);
        return NULL;
    }
    newNode->priority = queue->copyPriority(priority);
    if(newNode->priority == NULL)
    {
        free(newNode);
        return NULL;
    }
    return newNode;
}
/*!
* There are three places to store a new node in the queue:
* 1) Head of the queue -> if the new priority is the highest or if the queue is empty.
* 2) Buttom of the queue -> if the new priority is the lowest priority in the queue.
* 3) Middle of the queue -> all other opptions.
* therefore 3 functions to insert the new node at the right place:
*/

static PriorityQueueResult pqInsertNodeToHead(PriorityQueue queue, PQElement element,
                                           PQElementPriority priority, Node position)
{
    PQ_NULL_TEST(queue, PQ_OUT_OF_MEMORY)
    Node newNode = pqNodeCreate(queue, element, priority);
    PQ_NULL_TEST(newNode, PQ_OUT_OF_MEMORY)
    if(queue->head == NULL) 
    {
        queue->head = newNode;
    }
    else
    {
        newNode->next = queue->head;
        queue->head = newNode;
    }
    return PQ_SUCCESS;
}

static PriorityQueueResult pqInsertNodeToMiddle(PriorityQueue queue, PQElement element,
                                           PQElementPriority priority, Node position)
{
    PQ_NULL_TEST(queue, PQ_OUT_OF_MEMORY)
    Node newNode = pqNodeCreate(queue, element, priority);
    PQ_NULL_TEST(newNode, PQ_OUT_OF_MEMORY)
    newNode->next = position->next;
    position->next = newNode;
    return PQ_SUCCESS;
}

static PriorityQueueResult pqInsertNodeToButtom(PriorityQueue queue, PQElement element,
                                           PQElementPriority priority, Node position)
{
    PQ_NULL_TEST(queue, PQ_OUT_OF_MEMORY)
    Node newNode = pqNodeCreate(queue, element, priority);
    PQ_NULL_TEST(newNode, PQ_OUT_OF_MEMORY)
    position->next = newNode;
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
    PQ_NULL_TEST(queue->head, PQ_SUCCESS) // The queue is empty
    Node position = queue->head;
    while (position)
    {
        queue->freeElement(position->element);
        queue->freePriority(position->priority);
        Node toDelete = position;
        position = position->next;
        toDelete->next  = NULL;
        free(toDelete);
    }
    return PQ_SUCCESS; 
}

void pqDestroy(PriorityQueue queue)
{
    if(queue != NULL)
    {
        if(queue->head != NULL)
        {
            pqClear(queue);
        }
        queue->iterator = NULL;
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
        return newPriorityQueue;
    }
    Node position = queue->head;
    while(position) //* insert all of the queue nodes and the pqInsert function will arrange them correctly
    {
        if(pqInsert(newPriorityQueue,position->element,position->priority) != PQ_SUCCESS)
        {
            pqDestroy(newPriorityQueue);//if an element or priority copy won't succeed the new queue must be destroyed
            RESET_ITERATOR(queue)
            return NULL;
        }
        position = position->next;
    }
    RESET_ITERATOR(queue)
    RESET_ITERATOR(newPriorityQueue)
    return newPriorityQueue;
}

int pqGetSize(PriorityQueue queue)
{
    PQ_NULL_TEST(queue, INVALID)
    int counter = EMPTY;
    Node position = queue->head;
    while (position)
    {
        counter++;
        position = position->next;
    }
    return counter;
}

bool pqContains(PriorityQueue queue, PQElement element)
{
    PQ_NULL_TEST(queue, false)
    PQ_NULL_TEST(element, false)
    Node position = queue->head;
    while (position)
    {
        if (queue->equalElements(position->element, element))
        {
            return true;
        }
        position = position->next;
    }
    return false;
}

PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
    PQ_NULL_TEST(queue, PQ_NULL_ARGUMENT)    
    RESET_ITERATOR(queue)
    PQ_NULL_DATA_TEST(queue,element,PQ_NULL_ARGUMENT)
    PQ_NULL_DATA_TEST(queue,priority,PQ_NULL_ARGUMENT)
    if(queue->head==NULL) //* the queue is empty
    {
        return pqInsertNodeToHead(queue,element,priority,NULL); //add the new node to the head of the queue
    }
    if(queue->comparePriorities(priority, queue->head->priority) >= EQUAL)
    {
        //*insert the new node to the top
        return pqInsertNodeToHead(queue,element,priority,queue->head);
    }
    Node position = queue->head;
    while(position->next)
    {
        if(queue->comparePriorities(priority, position->next->priority) >= EQUAL)
        {
            //*insert the new node before the position founded */
            return pqInsertNodeToMiddle(queue,element,priority,position);
        }
        position = position->next;
    }
    return pqInsertNodeToButtom(queue,element,priority,position);//insert node to the button of the pq
}

PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element,
                                     PQElementPriority old_priority, PQElementPriority new_priority)
{
    PQ_NULL_TEST(queue, PQ_NULL_ARGUMENT)
    RESET_ITERATOR(queue)
    PQ_NULL_DATA_TEST(queue,element,PQ_NULL_ARGUMENT)
    PQ_NULL_DATA_TEST(queue,old_priority,PQ_NULL_ARGUMENT)
    PQ_NULL_DATA_TEST(queue,new_priority,PQ_NULL_ARGUMENT)
    Node position = queue->head;
    while(position)
    {
        if(queue->comparePriorities(position->priority, old_priority) > EQUAL)
        //* itarate the queue until reaches to the old priority
        {
            position = position->next;
            continue;
        }
        else if(queue->comparePriorities(position->priority, old_priority) == EQUAL)
        //**will replace the first node with the old priority
        {
            if(queue->equalElements(element, position->element)) //**will replace only an identical element
            {
                PriorityQueueResult removeResult = pqRemoveElement(queue, position->element);
                //* remove the element and insert a new one
                if(removeResult != PQ_SUCCESS) //* if the pqRemoveElement function didn't work
                {
                    return removeResult;
                }
                PriorityQueueResult insertResult = pqInsert(queue, element, new_priority);
                //* insert the new node with the new priority
                if(insertResult != PQ_SUCCESS)
                {
                    return insertResult;
                }
                return PQ_SUCCESS;
            }
            position = position->next;
            continue;
        }
        position = NULL;
    }
    return PQ_ELEMENT_DOES_NOT_EXISTS;
}

PriorityQueueResult pqRemove(PriorityQueue queue)
{
    PQ_NULL_TEST(queue, PQ_NULL_ARGUMENT)
    if(queue->iterator != NULL)
    {
        RESET_ITERATOR(queue)
    }    
    if(queue->head==NULL)
    {
        return PQ_SUCCESS;
    }
    queue->freeElement(queue->head->element);
    queue->freePriority(queue->head->priority);
    Node toDelete = queue->head;
    queue->head = queue->head->next;
    toDelete->next = NULL;
    free(toDelete);
    return PQ_SUCCESS; 
}

PriorityQueueResult pqRemoveElement(PriorityQueue queue, PQElement element)
{
    PQ_NULL_TEST(queue, PQ_NULL_ARGUMENT)
    RESET_ITERATOR(queue)
    if(queue->head == NULL) //* empty queue
    {
        return PQ_SUCCESS;
    }
    if(queue->equalElements(queue->head->element, element) == true)
    //* the first element of the queue needs to be removed
    {
        return pqRemove(queue);
    }
    else if(queue->head->next == NULL)
    //* the queue contains one node that don't need to be removed
    {
        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }
    Node position = queue->head;
    while(position->next)
    {
        if(queue->equalElements(position->next->element, element) == true)
        //*compare the next element, save the previus node to patch the queue after the delete
        {
            queue->freeElement(position->next->element);
            queue->freePriority(position->next->priority);
            Node toDelete = position->next;
            position->next = position->next->next;
            toDelete->next = NULL;
            free(toDelete);
            return PQ_SUCCESS; 
        }
        position = position->next;
    }
    return PQ_ELEMENT_DOES_NOT_EXISTS; 
}

PQElement pqGetFirst(PriorityQueue queue)
{
    PQ_NULL_TEST(queue, NULL)
    if(queue->head == NULL)
    {
        RESET_ITERATOR(queue)
        return NULL;
    }
    queue->iterator = queue->head;
    return queue->head->element;
}

PQElement pqGetNext(PriorityQueue queue)
{
    PQ_NULL_TEST(queue, NULL)
    PQ_NULL_TEST(queue->iterator, NULL)
    else if(queue->iterator->next == NULL)
    {
        RESET_ITERATOR(queue)
        return NULL;
    }
    queue->iterator = queue->iterator->next;
    return queue->iterator->element;
}
