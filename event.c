#include "event.h"
#include "date.h"
#include "member.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event_manager.h"
#include "priority_queue.h"

struct Event_t
{
    char* event_name;
	int event_id;
    Date date;
	PriorityQueue members_per_event;
};

static PQElement copy_member(PQElement elem)
{
	return memberCopy((Member)elem);
}

static void free_member(PQElement elem)
{
	memberDestroy(elem);
}

static bool equal_member(PQElement elem1,PQElement elem2)
{
	return memberCompare(elem1,elem2);
}

static int compare_member_id(PQElementPriority priority1, PQElementPriority priority2)
{
	return memberIdCompare(priority1, priority2);
} 

Event eventCreate(char* name, int id, Date date)
{
    if(name ==  NULL || date == NULL)
    {
        return NULL;
    }
    Event event = malloc(sizeof(*event));
    if (event == NULL) 
    {
        return NULL;
    }
    event->event_name = malloc(strlen(name) + 1);
    if (event->event_name == NULL) 
    {
        return NULL;
    }
    strcpy(event->event_name, name);
    event->event_id = id;
    event->members_per_event = pqCreate(copy_member, free_member, equal_member, copy_member, free_member,
                                        compare_member_id);
    if(event->members_per_event == NULL)
    {
        eventDestroy(event);
        return NULL;
    }
    event->date = dateCopy(date);
    return event;
}

//Creates a copy of target event
Event eventCopy(Event event)
{
    if(event == NULL)
    {
        return NULL;
    }
    Event event_copy = eventCreate(event->event_name, event->event_id, event->date);
    event_copy->members_per_event = pqCopy(event->members_per_event);
    return event_copy;
}

//Deallocates an existing Date
void eventDestroy(Event event)
{
        if(event == NULL)
        {
            return;
        }
        dateDestroy(event->date);
        free(event->event_name);
        pqDestroy(event->members_per_event);
        free(event);
}

bool eventCompare(Event event1, Event event2)
{
    if((event1 == NULL) || (event2 == NULL))
    {
        return NULL;
    }
    int id_res = ((event1->event_id) - (event2->event_id));
    if(!id_res)
    {
        return true;
    }
    return false;
}

int getEventId(Event event)
{
    return event->event_id;
}

char* getEventName(Event event)
{
    if(event == NULL)
    {
        return NULL;
    }
    return event->event_name;
}

PriorityQueue getMembersPerEvent(Event event)
{
    if(event == NULL)
    {
        return NULL;
    }
    return event->members_per_event;
}

Date getEventdate(Event event)
{
    if(event == NULL)
    {
        return NULL;
    }
    return event->date;
}

char* getNameFromEvent(Event event)
{
    return event->event_name;
}

int eventDateCompare(Event event1, Event event2)
{
    return dateCompare(event2->date, event1->date);
}

Event eventDateChange(Event event, Date new_date)
{
    if(event == NULL)
    {
        return NULL;
    }
    Event new_event = eventCopy(event);
    new_event->date = dateCopy(new_date);
    return new_event;
}