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
    Node newNode = malloc(sizeof(newNode));
    if (newNode == NULL)
    { 
        return NULL;
    }
    newNode->element = NULL;
    newNode->priority = NULL;
    newNode->next = NULL;
    return newNode;
}

PriorityQueue pqCreate(CopyPQElement copy_element, FreePQElement free_element, 
                       EqualPQElements equal_elements, CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority, ComparePQElementPriorities compare_priorities)
{
    PriorityQueue newPriorityQueue = malloc(sizeof(newPriorityQueue));
    if (newPriorityQueue == NULL)
    {
        return NULL;
    }
    Node headNode = nodeCreate();
    newPriorityQueue->head              = headNode; //* the new node has null values by defenition
    newPriorityQueue->iterator          = NULL;
    //* functions */
    newPriorityQueue->copyElement       = copy_element;
    newPriorityQueue->freeElement       = free_element;
    newPriorityQueue->equalElements     = equal_elements;
    newPriorityQueue->copyPriority      = copy_element;
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
    Node toDelete = head;
    while (head->next)
    {
        queue->freeElement(head->element);
        queue->freePriority(head->priority);
        head = head->next;
        free(toDelete); //free the current node because after the clear function the head will point to the first node
    }
    return PQ_SUCCESS; 
}

void pqDestroy(PriorityQueue queue)
{
    if(queue != NULL)
    {
        pqClear(queue);
        //free(queue->head); // need to free the last node
        //if(queue->iterator != NULL) free(queue->iterator); //* mostly the iterator will be pointing to the head node = NULL
        //free(queue);
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
    Node current = queue->head, newList = NULL, tail = NULL;
    newList = nodeCreate();
    newList->element = queue->copyElement(current->element);
    newList->priority = queue->copyPriority(current->priority);
    tail = newList;
    while(current->next)
    {
        tail->next = nodeCreate();
        tail = tail->next;
        tail->element = queue->copyElement(current->element);
        tail->priority = queue->copyPriority(current->priority);
        current = current->next;
    }
    newPriorityQueue->head = newList;
    free(newList);
    free(tail);
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
    while (head->next)
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
    while (head->next)
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
    while(head->next)
    {
        if(queue->comparePriorities(priority, head->priority) != PRIOR) //**will insert the new node after an equal or bigger priorty element
        {
            Node newNode = nodeCreate();
            newNode->element = queue->copyElement(element);
            COPY_ELEMENT_TEST;
            newNode->priority = queue->copyPriority(priority);
            COPY_PRIORITY_TEST;
            return PQ_SUCCESS;
        }
        head = head->next;
    }
    Node newNode = nodeCreate();
    newNode->element = queue->copyElement(element);
    COPY_ELEMENT_TEST;
    newNode->priority = queue->copyPriority(priority);
    COPY_PRIORITY_TEST;
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
    while(head->next)
    {
        if(queue->comparePriorities(head->priority, old_priority) == PRIOR)
        {
            head = head->next;
            continue;
        }
        else if(queue->comparePriorities(head->priority, old_priority) == EQUAL) //**will replace the first node with the old priority, ill get in this block if i didnt pass the old priority
        {
            if(queue->equalElements(element, head->element)) //**will replace only an identical element
            {
                Node temp = head;
                PriorityQueueResult removeResult = pqRemoveElement(queue, head->element);
                if(removeResult != PQ_SUCCESS)
                {
                    return removeResult;
                }
                PriorityQueueResult insertResult = pqInsert(queue, temp->element, temp->priority);
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
    void* highest_Priority = NULL;
    while(head->next)
    {
        highest_Priority = head->priority;
        if(queue->equalElements(queue->head->element, element))
        {
            highest_Priority = head->priority;
            queue->freeElement(queue->head->element);
            queue->freePriority(queue->head->priority);
            queue->head = queue->head->next;
            if(queue->comparePriorities(head->priority, highest_Priority) != EQUAL)
            {
                return PQ_SUCCESS; 
            }
        }
        head = head->next;
    }
    return PQ_ELEMENT_DOES_NOT_EXISTS; 
}

PQElement pqGetFirst(PriorityQueue queue)
{
    if(queue == NULL)
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


