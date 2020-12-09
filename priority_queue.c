#include "priority_queue.h"
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#define FALSE 0
#define TRUE 1
#define INVALID -1
#define EMPTY 0
#define PRIOR -1
#define EQUAL 0

#define COPY_ELEMENT_TEST if (newNode->element == NULL) return PQ_OUT_OF_MEMORY;
#define COPY_PRIORITY_TEST if (newNode->priority == NULL) return PQ_OUT_OF_MEMORY;

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

static Node nodeCreate()
{
    Node newNode = malloc(sizeof(*newNode));
    if (newNode == NULL)
    { 
        return NULL;
    }
    newNode->element = NULL;
    newNode->priority = NULL;
    newNode->next = NULL;
    return newNode;
}

//* insert a new node to the linked list, will insert the new node to the address the 'ptr' points at.

static PriorityQueueResult insertNewNode(PriorityQueue queue, PQElement element, PQElementPriority priority, Node head)
{
    Node newNode = nodeCreate();
    newNode->element = queue->copyElement(element);
    COPY_ELEMENT_TEST;
    newNode->priority = queue->copyPriority(priority);
    COPY_PRIORITY_TEST;
    if(head==NULL)
    {
        queue->head = newNode;
        queue->head->next = NULL;
    }
    else if (head->next==NULL)
    {
        queue->head->next = head;
        queue->head = newNode;
    }
    else
    {
        newNode->next = head->next;
        head->next = newNode;
    }

    return PQ_SUCCESS;
} 

PriorityQueue pqCreate(CopyPQElement copy_element, FreePQElement free_element, 
                       EqualPQElements equal_elements, CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority, ComparePQElementPriorities compare_priorities)
{
    PriorityQueue newPriorityQueue = malloc(sizeof(*newPriorityQueue));
    if (newPriorityQueue == NULL)
    {
        return NULL;
    }
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
    if(queue == NULL)
    {
        return PQ_NULL_ARGUMENT; 
    }
    Node head = queue->head;
    if(head==NULL) //the queue is empty
    {
        return PQ_SUCCESS; 
    }
    else if(head->next == NULL) //the queue has only one node
    {
        queue->freeElement(queue->head->element);
        queue->freePriority(queue->head->priority);
        free(queue->head);
        return PQ_SUCCESS; 
    }
    while (head) //the queue has more then one nodem free elements, priorities and the p
    {
        queue->freeElement(head->element);
        queue->freePriority(head->priority);
        head = head->next;
        free(queue->head); 
    }
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
    if(queue == NULL)
    {
        return NULL; 
    }
    PriorityQueue newPriorityQueue = pqCreate(queue->copyElement, queue->freeElement, queue->equalElements,
                                                queue->copyPriority, queue->freePriority, queue->comparePriorities);
    if(queue->head == NULL)
    {
            return newPriorityQueue;
    }
    Node head = queue->head, tail = NULL, newNode = NULL, ptr=NULL;
    tail = nodeCreate(); //the node that will add the head->next according to the newNode address
    tail->element = queue->copyElement(head->element);
    tail->priority = queue->copyPriority(head->priority);
    ptr = tail; // pointer to the head of the new PQ
    while(head)
    {
        newNode = nodeCreate();
        tail->next = newNode; // nesseccery because we need the 'next' node but we are copying upside down
        newNode->element = queue->copyElement(head->element);
        newNode->priority = queue->copyPriority(head->priority);
        tail = tail->next; //get the tail point to the same address as the newNode
        head = head->next;
    }

    newPriorityQueue->head = ptr; // the new pq->head will point to the highest priority of the linked list
    return newPriorityQueue;
}

int pqGetSize(PriorityQueue queue)
{
    if (queue == NULL)
    {
        return INVALID;
    }
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
    if (queue==NULL || element==NULL)
    {
        return FALSE;
    }
    Node head = queue->head;
    while (head)
    {
        if (queue->equalElements(queue->head->element, element))
        {
            return TRUE;
        }
        head = head->next;
    }
    return FALSE;
}

PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
    if(queue == NULL || element == NULL || priority == NULL)
    {
        return PQ_NULL_ARGUMENT; 
    }
    Node head = queue->head;
    if (head==NULL)
    {
        return insertNewNode(queue,element,priority,head); // special ccase that the queue is empty
    }
    if (head->next == NULL)
    {
        return insertNewNode(queue,element,priority,head->next); //second special case that there is only one node in the queue
    }
    while(head)
    {
        if(queue->comparePriorities(head->next->priority, priority) <= EQUAL) //**will insert the new node after an equal or bigger priorty element
        {
            insertNewNode(queue,element,priority,head);
        }
        head = head->next;
    }
    return PQ_SUCCESS;
}

PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element,
                                     PQElementPriority old_priority, PQElementPriority new_priority)
{
    if(queue == NULL || element == NULL || old_priority == NULL || new_priority == NULL)
    {
        return PQ_NULL_ARGUMENT; 
    }
    Node head = queue->head;
    while(head)
    {
        if(queue->comparePriorities(head->priority, old_priority) < EQUAL)
        {
            head = head->next;
            continue;
        }
        else if(queue->comparePriorities(head->priority, old_priority) == EQUAL) //**will replace the first node with the old priority, ill get in this block if i didnt pass the old priority
        {
            if(queue->equalElements(element, head->element)) //**will replace only an identical element
            {
                PriorityQueueResult removeResult = pqRemoveElement(queue, head->element);
                if(removeResult != PQ_SUCCESS)
                {
                    return removeResult;
                }
                PriorityQueueResult insertResult = pqInsert(queue, element, new_priority);
                if(insertResult != PQ_SUCCESS)
                {
                    return insertResult;
                }
                queue->iterator = NULL;
                return PQ_SUCCESS;
            }
            head = head->next;
            continue;
        }
        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }
    return PQ_ELEMENT_DOES_NOT_EXISTS;
}

PriorityQueueResult pqRemove(PriorityQueue queue)
{
    if(queue == NULL)
    {
        return PQ_NULL_ARGUMENT; 
    }
    Node head = queue->head;
    void* highest_Priority = queue->head->priority;
    while(head->next)
    {
        if(queue->comparePriorities(head->priority, highest_Priority) == PRIOR)
        {
            continue;
        }
        else if(queue->comparePriorities(head->priority, highest_Priority) == EQUAL) //**will remove the highest priority elements from the queue
        {
            queue->freeElement(queue->head->element);
            queue->freePriority(queue->head->priority);
            queue->head = queue->head->next;
        }
        head = head->next;
    }
    return PQ_SUCCESS; 
}

PriorityQueueResult pqRemoveElement(PriorityQueue queue, PQElement element)
{
    if(queue == NULL)
    {
        return PQ_NULL_ARGUMENT; 
    }
    Node head = queue->head;
    while(head)
    {
        if(queue->equalElements(queue->head->element, element))
        {
            queue->freeElement(queue->head->element);
            queue->freePriority(queue->head->priority);
            queue->head = queue->head->next;
            return PQ_SUCCESS; 

        }
        head = head->next;
    }
    return PQ_ELEMENT_DOES_NOT_EXISTS; 
}

PQElement pqGetFirst(PriorityQueue queue)
{
    if(queue == NULL || queue->head == NULL)
    {
        return NULL; 
    }
    queue->iterator = queue->head;
    return queue->head->element;
}

PQElement pqGetNext(PriorityQueue queue)
{
    if(queue == NULL || queue->head->next == NULL)
    {
        return NULL; 
    }
    queue->iterator = queue->head->next;
    return queue->iterator;
}




