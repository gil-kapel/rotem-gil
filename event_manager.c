
struct event
{
    int id;
    int name;
    struct date;
}

struct member
{
    int id;
    int name;
    int how_many_events?
}

struct EventManager_t
{
    PriorityQueue Event_list = pqCreate(,,,,,,);
    PriorityQueue Members_list = pqCreate(,,,,,,)
}

pqInsert(Event_list, member, member->how_many_times);


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_manager.h"
#define VALID_DATE 0
#define VALID_ID 0
#define EQUAL 0

typedef struct event
{
	int event_id;
	char* event_name;
	int member_id;
	char* member_name;
	//Date date;
	struct event* next;
}*Event;

struct EventManager_t 
{
    //EventManager_tElement* events;
    Date date;
	Event event;
	struct EventManager_t* next;
};

//Allocates a new event managers
EventManager createEventManager(Date date)
{
	EventManager em = malloc(sizeof(*em));
	if(em == NULL) 
    {
		return NULL;
	}
	em->date = date;
	em->next = NULL;
	return em;
}

//Deallocates an existing event manager
void destroyEventManager(EventManager em)
{
    if(em == NULL) 
    {
		return;
	}
	EventManager em_to_delete = em;
	em = em->next;
	free(em_to_delete);    
}

//Compares ID of two events
static bool eventIdCompare(const int event_ID_1, const int event_ID_2)
{
	if((event_ID_1) == (event_ID_2))
	{
		return true;
	}
	return false;
}

//Find event with the same name
static bool eventFind_ID(const int event_id_1, EventManager em)///CHANGE TO INT!!!!!!!!
{
	for (Event ptr = (em->event); ptr != NULL; ptr = ptr->next)
	{
		int new_id = ptr->event_id;
		if (eventIdCompare(event_id_1,new_id))
		{
			return true;
		}
	}
		return false;
}
//Compares names of two events
static bool eventNameCompare(const char* event_name_1, const char* event_name_2)
{
	if(strcmp(event_name_1, event_name_2) == EQUAL)
	{
		return true;
	}
	return false;
}


//Find event with the same name
static bool eventFindName(const char* event_name_1, EventManager em)///
{
	for (Event ptr = (em->event); ptr != NULL; ptr = ptr->next)
	{
		char* new_name = ptr->event_name;
		if (eventNameCompare(event_name_1,new_name))
		{
			return true;
		}
	}
		return false;
}


//Adds new event to existing date
EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id)
{
	if((em == NULL) || (event_name == NULL) || (date == NULL))
	{
		return EM_NULL_ARGUMENT;
	}
	Date current_date = em->date;
	if (dateCompare(date, current_date) < VALID_DATE)
	{
		return EM_INVALID_DATE;
	}
	if (event_id < VALID_ID)
	{
		return EM_INVALID_EVENT_ID;
	}
	if(eventFindName(event_name,em))
	{
		return EM_EVENT_ALREADY_EXISTS;
	}
	if(eventFind_ID(event_id, em))
	{
		return EM_EVENT_ID_ALREADY_EXISTS;
	}
	///OTHER CASES
	////
	///AT THE END SUCESS
	return EM_SUCCESS;
}
