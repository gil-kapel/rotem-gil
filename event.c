#include "event.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event_manager.h"
#include "priority_queue.h"
#include "date.h"
#include "member.h"

struct Event_t
{
    char* event_name;
	int event_id;
    Date date;
	PriorityQueue members_per_event;//?
};

static PQElement copy_member(PQElement elem)
{
	return memberCopy(elem);
}

static void free_member(PQElement elem)
{
	memberDestroy(elem);
}


static bool equal_member(PQElement elem1,PQElement elem2)
{
	return memberCompare(elem1,elem2);
}

static PQElementPriority copy_member_id(PQElementPriority priority)//check it out
{
	return copyMemberId(priority);
}

static void free_member_id(PQElementPriority priority)
{
	freeMemberId(priority);
}

static int compare_member_id(PQElementPriority priority1, PQElementPriority priority2)
{
	return memberIdCompare(priority1, priority2);
} 

static char* stringCopy(char* new_str)
{
    if(!new_str)
    {
        return NULL;
    }
    char* copy_str = malloc(strlen(new_str)+1);
    return copy_str ? strcpy(copy_str, new_str) : NULL;
}

Event eventCreate(char* name, int id, Date date)//,  members_per_event)// change in em
{
    Event event = malloc(sizeof(*event));
    if (event == NULL) 
    {
        return NULL;
    }
    //event->event_name = malloc(strlen(name)+1);
    event->event_name = stringCopy(name);
    if (event->event_name == NULL) 
    {
        eventDestroy(event);//
        return NULL;
    }
    event->event_id = id;
    PriorityQueue members_per_event = pqCreate(copy_member, free_member, equal_member, copy_member_id, free_member_id, compare_member_id);
    event->date = date;
    //event->members_per_event = pqCopy(members_per_event);
    return event;
}

//Creates a copy of target event
Event eventCopy(Event event)
{
    if(event == NULL)
    {
        return NULL;
    }
    return eventCreate(event->event_name, event->event_id, event->date);
}

//Deallocates an existing Date
void eventDestroy(Event event)
{
    if(event == NULL)
    {
        return;
    }
    free(event->event_name);//
    free(event->members_per_event);//what else?
    free(event);
}

bool eventCompare(Event event1, Event event2)
{
    if((event1 == NULL) || (event2 == NULL))
    {
        return NULL;
    }
    int name_res = strcmp(event1->event_name,event2->event_name);
    int id_res = ((event1->event_id) - (event2->event_id));
    int date_res = dateCompare(event1->date, event2->date);
    if((!name_res) && (!id_res) && (!date_res))
    {
        return true;
    }
    return false;
}

int getEventId(Event event)
{
    return event->event_id;
}

char* getEventName(Event event)//
{
    return event->event_name;
}

Date getEventdate(Event event)
{
    return event->date;
}

char* getNameFromEvent(Event event)
{
    return stringCopy(event->event_name);
}

// bool compareEventsId(Event event1, Event event2)
// {
//     if ((event1->event_id) == (event2->event_id))
//     {
//         return true;
//     }
//     return false;
// }




// bool eventGet(Event event, char** name, int* id, Date date)
// {
//     if((name == NULL) || (id == NULL) || (date == NULL))
//     {
//         return false;
//     }
//     *name = event->event_name; 
//     *id = event->event_id; 
//     dateGet(Date date, int* day, int* month, int* year)
//     *date = event->date;
//     // date->day = event->date->day;
//     // (date->month) = event->date->month;
//     // (date->year) = event->date->year;

//     return true;
// }

// copy_element(PQElement event)
// {
// 	Event new_event = eventCopy((Event)event);
// 	return new_event;

// }
