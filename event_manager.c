#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "event_manager.h"
#include "priority_queue.h"
#include "event.h"
#include "date.h"
#include "member.h"
#define VALID_DATE 0
#define VALID_ID 0
#define EQUAL 0
#define DAY 1


struct EventManager_t 
{
	PriorityQueue events;
	PriorityQueue members;
	Date first_date;//NULL?
};

// typedef struct priorityById_t
// {
// 	int id;
// }PriorityID;


// typedef struct priorityByQ_t
// {
// 	Date date;
// }PriorityDate;

//events functions

static PQElement copyEventInEm(PQElement elem)
{
	return eventCopy(elem);
}

static void freeEventInEm(PQElement elem)
{
	eventDestroy(elem);
}


static bool equalEventInEm(PQElement elem1,PQElement elem2)
{
	return eventCompare(elem1,elem2);
}

static PQElementPriority copyDatePriority(PQElementPriority priority)
{
	return dateCopy(priority);
}

static void freeDatePriority(PQElementPriority priority)
{
	dateDestroy(priority);
}

static int compareDatePriority(PQElementPriority priority1, PQElementPriority priority2)
{
	return dateCompare(priority1, priority2);
} 

//members functions

static PQElement copymemberInEm(PQElement elem)
{
	return memberCopy(elem);
}

static void freeMemberInem(PQElement elem)
{
	memberDestroy(elem);
}

static bool equalMembersInEm(PQElement elem1,PQElement elem2)
{
	return memberCompare(elem1,elem2);
}

static PQElementPriority copyMemberAmount(PQElementPriority priority)
{
	return copyAmount(priority);
}

static void freeMemberAmount(PQElementPriority priority)
{
	amountDestroy(priority);
}

static int compareMemberAmount(PQElementPriority priority1, PQElementPriority priority2)
{
	return amountCompare(priority1, priority2);
} 


//Allocates a new event managers
EventManager createEventManager(Date date)
{
	EventManager em = malloc(sizeof(*em));
	if(em == NULL) 
    {
		return NULL;
	}
	em->events = pqCreate(copyEventInEm,freeEventInEm,equalEventInEm,copyDatePriority,freeDatePriority,compareDatePriority);
	if(!(em->events))
	{
		return NULL;
	}
	em->members = pqCreate(copymemberInEm,freeMemberInem,equalMembersInEm,copyMemberAmount,freeMemberAmount,compareMemberAmount);
	em->first_date = date;
	return em;
}


//Deallocates an existing event manager
void destroyEventManager(EventManager em)
{
	if(em == NULL)
	{
		return;
	}
	pqDestroy(em->events);
	//
	dateDestroy(em->first_date);
	free(em);
}

// static Event getEventFromName(EventManager em, char* name)
// {
// 	PQ_FOREACH(Event, current, em->events)
// 	if(!strcmp(getEventName(current), name))
// 	{
// 		return current;
// 	}
// 	return NULL;
// }

static bool FindEventFromId(EventManager em, int id)
{
	PQ_FOREACH(Event, current, em->events)
	if ((getEventId(current)) == id)
	{
		return true;
	}
	return false;
}

// static bool eventNameinSameDate(EventManager em, char* event_name, Date date)
// {
// 	Event event_in_em = (getEventFromName(em, event_name));
// 	if(event_in_em)
// 	{
// 		Date date_in_em = (getEventdate(event_in_em));
// 		if(!dateCompare(date_in_em, date))
// 		{
// 			return true;
// 		}
// 	}
// 	return false;
// }


static bool eventNameinSameDate(EventManager em, char* event_name, Date date)
{
	PQ_FOREACH(Event, current, em->events)
	if (!dateCompare(date,getEventdate(current))) 
	{
		if(strcmp(event_name, getEventName(current)))
		{
			return true;
		}
	}
	return false;
}


EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id)
{
	if((em == NULL) || (event_name == NULL) || (date == NULL))
	{
		return EM_NULL_ARGUMENT;
	}
	if(dateCompare(date, em->first_date) < VALID_DATE)	
	{
		return EM_INVALID_DATE;
	}
	if(event_id < VALID_ID)
	{
		return EM_INVALID_EVENT_ID;
	}
	if(eventNameinSameDate(em, event_name, date))
	{
		return EM_EVENT_ALREADY_EXISTS;
	}
	if(FindEventFromId(em, event_id))
	{
		return EM_EVENT_ID_ALREADY_EXISTS;
	}
	Event new_event = eventCreate(event_name, event_id, date);
	if(!new_event)
	{
		destroyEventManager(em);
		return EM_OUT_OF_MEMORY;
	}
	PriorityQueueResult res = pqInsert(em->events, new_event, date);// what happened when events is NULL
	if(res == PQ_OUT_OF_MEMORY)
	{
		destroyEventManager(em);
		return EM_OUT_OF_MEMORY;
	}
	return EM_SUCCESS;
}

static Date tickDateByDays(EventManager em, int days)
{
	Date chosen_date = dateCopy(em->first_date);
	while (days > VALID_DATE)
	{
		dateTick(chosen_date);
		days = days - DAY;
	}
	return chosen_date;
}


EventManagerResult emAddEventByDiff(EventManager em, char* event_name, int days, int event_id)
{
	if((em == NULL) || (event_name == NULL))
	{
		return EM_NULL_ARGUMENT;
	}
	if(days < VALID_DATE)	
	{
		return EM_INVALID_DATE;
	}
	if(event_id < VALID_ID)
	{
		return EM_INVALID_EVENT_ID;
	}
	Date date = tickDateByDays(em, days);
	EventManagerResult res = emAddEventByDate(em, event_name, date, event_id);
	free(date);
	return res;
}


static Event getEventFromId(EventManager em, int id)
{
	PQ_FOREACH(Event, current, em->events)
	{
		if ((getEventId(current)) == id)
		{
			return current;
		}
	}
	return NULL;
}

EventManagerResult emRemoveEvent(EventManager em, int event_id)
{
	if (em == NULL) 
	{
		return  EM_NULL_ARGUMENT;
	}
	if(event_id < VALID_ID)
	{
	return EM_INVALID_EVENT_ID;
	}
	Event event_to_remove = getEventFromId(em, event_id);
	if(!event_to_remove)
	{
		return EM_EVENT_NOT_EXISTS;
	}
	//freeEventInEm(em);
	PriorityQueueResult res = pqRemoveElement(em->events, event_to_remove);
	if (res == PQ_OUT_OF_MEMORY)
	{
		destroyEventManager(em);
		return EM_OUT_OF_MEMORY;
	}
	return EM_SUCCESS;
}

EventManagerResult emChangeEventDate(EventManager em, int event_id, Date new_date)
{
	if((em == NULL)  || (new_date == NULL))
	{
		return  EM_NULL_ARGUMENT;
	}
	if(dateCompare(new_date, em->first_date) < VALID_DATE)	
	{
		return EM_INVALID_DATE;
	}
	if(event_id < VALID_ID)
	{
		return EM_INVALID_EVENT_ID;
	}
	if(!FindEventFromId(em, event_id))
	{
		return EM_EVENT_ID_NOT_EXISTS;
	}
	Event event_for_change = getEventFromId(em, event_id);
	char* name = getNameFromEvent(event_for_change);
	Date old_date = getEventdate(event_for_change);
	if(eventNameinSameDate(em, name, new_date))
	{
		return EM_EVENT_ALREADY_EXISTS;
	}
	PriorityQueueResult res = pqChangePriority(em->events, event_for_change, old_date, new_date);
	if(res == PQ_OUT_OF_MEMORY)
	{
		destroyEventManager(em);
		return EM_OUT_OF_MEMORY;
    }
		return EM_SUCCESS;
} 


static bool findMemberById(EventManager em, int member_id)
{
		PQ_FOREACH(Member, current, em->members)
	if ((*getMemberId(current)) == member_id)
	{
		return true;
	}
	return false;
}

static Member getMemberById(EventManager em, int member_id)
{
	PQ_FOREACH(Member, current, em->members)
	{
		if ((*getMemberId(current)) == member_id)
		{
			return current;
		}
	}
	return NULL;
}

EventManagerResult emAddMember(EventManager em, char* member_name, int member_id)
{
	if((em == NULL)  || (member_name == NULL))
	{
		return EM_NULL_ARGUMENT;
	}
	if(member_id < VALID_ID)
	{
		return EM_INVALID_MEMBER_ID;
	}
	if(findMemberById(em,member_id))
	{
		return EM_MEMBER_ID_ALREADY_EXISTS;
	}
	Member new_member = memberCreate(member_name, member_id);
	if(!new_member)
	{
		destroyEventManager(em);//is it really necessery
		return EM_OUT_OF_MEMORY;
	}
	PriorityQueueResult res = pqInsert(em->members, new_member, member_id);// what happened when events is NULL
	if(res == PQ_OUT_OF_MEMORY)
	{
		destroyEventManager(em);
		return EM_OUT_OF_MEMORY;
	}
	return EM_SUCCESS;
}


static bool eventWithSameMember(EventManager em, int member_id, int event_id)
{
	Event chosen_event = getEventFromId(em, event_id);
	PQ_FOREACH(Member, iterator, em->events->chosen_event->members_per_event)//can we point on a queue and than on an elem inside the queue
	{
		if ((getEventId(iterator)) == member_id)
		{
			return true;
		}
	}
	return false;
}

//function that check if exist 2 loops
EventManagerResult emAddMemberToEvent(EventManager em, int member_id, int event_id)
{
	if(em == NULL)
	{
		return EM_NULL_ARGUMENT;
	}
	if(event_id < VALID_ID)
	{
	return EM_INVALID_EVENT_ID;
	}
	if(member_id < VALID_ID)
	{
	return EM_INVALID_MEMBER_ID;
	}
	if(!findMemberById(em, member_id))
	{
		return EM_MEMBER_ID_NOT_EXISTS;
	}
	if(eventWithSameMember(em, member_id, event_id))
	{
		return EM_EVENT_AND_MEMBER_ALREADY_LINKED;
	}
	Member new_member = getMemberById(em, member_id);
	PriorityQueueResult res = pqInsert(em->events->members_per_event, new_member, member_id);//is it ok to put em->events->otherqueue?
	if(res == PQ_OUT_OF_MEMORY)
	{
		destroyEventManager(em);
		return EM_OUT_OF_MEMORY;
	}
	return EM_SUCCESS;
}


EventManagerResult emRemoveMemberFromEvent(EventManager em, int member_id, int event_id)
{
	if(em == NULL)
	{
		return EM_NULL_ARGUMENT;
	}
	if(event_id < VALID_ID)
	{
	return EM_INVALID_EVENT_ID;
	}
	if(member_id < VALID_ID)
	{
	return EM_INVALID_MEMBER_ID;
	}
	if(!findMemberById(em, member_id))
	{
		return EM_MEMBER_ID_NOT_EXISTS;
	}
	if(!eventWithSameMember(em, member_id, event_id))
	{
		return EM_EVENT_AND_MEMBER_NOT_LINKED;
	}
	Member member_to_delete = getMemberById(em, member_id);
	PriorityQueueResult res = pqRemoveElement(em->events->members_per_event, member_to_delete);
	if (res == PQ_OUT_OF_MEMORY)
	{
		destroyEventManager(em);
		return EM_OUT_OF_MEMORY;
	}
	return EM_SUCCESS;

}

static PriorityQueueResult remove_prev_events(EventManager em, Date date)
{
	//PQ_FOREACH(Event, current, em->events)
	while(dateCompare(getEventdate(pqGetFirst(em->events)), date) < 0)
	{
		PriorityQueueResult res = pqRemove(pqGetFirst(em->events));// if fail check pqremoveelement
		if(res == PQ_NULL_ARGUMENT)
		{
			return EM_NULL_ARGUMENT;
		}
	}
	return EM_SUCCESS;
}



EventManagerResult emTick(EventManager em, int days)
{
	if(em == NULL)
	{
		return EM_NULL_ARGUMENT;
	}
	if(days <= 0)
	{
		return EM_INVALID_DATE;
	}
	Date new_date = tickDateByDays(em, days);
	Date to_delete = em->first_date;
	em->first_date = new_date;
	free(to_delete);
	return remove_prev_events(em, new_date);
}


int emGetEventsAmount(EventManager em)
{
	return pqGetSize(em->events);
}

char* emGetNextEvent(EventManager em)
{
	if(em == NULL)
	{
		return NULL;
	}
	return pqGetNext(em->events);
}

void printMembersPerEvent(Event event, const char* file_name)//print members per event
{
	PQ_FOREACH(Member, current, event->members_per_event)
	{
		fprintf(file_name,"%s, ", current);//how to put only one comma at the end

	}
}

void printCurrentEvent(Event event, const char* file_name)
{
	fprintf(file_name,"%s, ", event->event_name);
	fprintf(file_name, "%d.%d.%d, ", event->date->day, event->date->month, event->date->year);
	printMembersPerEvent(event, file_name);
}


void emPrintAllEvents(EventManager em, const char* file_name)
{
	FILE* file_name = fopen(file_name, "a");
	if(!file_name)//filename or file_name?, create file already?
	{
		return;
	}
	PQ_FOREACH(Event, current, em->events)//can we create loop in loop?
	{
		printCurrentEvent(current, file_name);
		fprintf(file_name, "\n");// is 'a' needed again?
	}
	fclose(file_name);	
}

void printCurrentMember()
{
	fprintf(file_name, "%s, ", member->member_name);// is a or w is needed?
	fprintf(file_name,"%d, ", member->member_name);
	printMembersPerEvent(event);//???
}


void emPrintAllResponsibleMembers(EventManager em, const char* file_name)//only a or w?
{
	FILE* file_name = fopen(file_name, "a");//filename or file_name?, create file already?
	if(!file_name)
	{
		return;
	}
	PQ_FOREACH(Member, current, em->members)//can we create loop in loop?
	{
		if(*(current->amount) > 0) //
		{
		if(memberIdCompare(*(current->member_id), *getMemberId(pqGetNext(em->members))))
		{
			//write printCurrentMember
		}
		printCurrentMember(current, file_name);//how to know what the events' size of each member
		fprintf(file_name, "\n");// is 'a' needed again?
		}
	}
	fclose(file_name);	
}












// if(days == VALID_DATE)
	// {
	// current_date = em->first_date;
		//emAddEventByDate(em, event_name, em->first_date, event_id);
	//}
	//Date current_date = em->first_date;



// static EventManagerResult checkErorrInAddEvent(EventManager em, char* event_name, Date date, int event_id)
// {
// 	if((em == NULL) || (event_name == NULL) || (date == NULL))
// 	{
// 		return EM_NULL_ARGUMENT;
// 	}
// 	if(dateCompare(date, em->first_date) < VALID_DATE)	
// 	{
// 		return EM_INVALID_DATE;
// 	}
// 	if(event_id < VALID_ID)
// 	{
// 		return EM_INVALID_EVENT_ID;
// 	}
// 	if(eventNameinSameDate(em, event_name, date))
// 	{
// 		return EM_EVENT_ID_ALREADY_EXISTS;
// 	}
// 	if(FindEventFromId(em, event_id))
// 	{
// 		return EM_EVENT_ID_ALREADY_EXISTS;
// 	}
// }

//PriorityQueue events_list = pqCreate(Event->copyElement, Event->freeElement, Event->equalElements,
//                                                Event->copyPriority, Event->freePriority, Event->comparePriorities)

//PQElement copy_element(Event event);
//{
//	return dateCopy();
//}



//Allocates a new event managers
// EventManager createEventManager(Date date)
// {
// 	//return pqCreate(em copy, )
// 	 return calCreate(hourEventCopy, hourEventDestroy);
// }
// EventManager createEventManager(Date date)
// {
// 	EventManager em = malloc(sizeof(*em));
// 	if(em == NULL) 
//     {
// 		return NULL;
// 	}
// 	em->date = date;
// 	em->next = NULL;
// 	return em;
// }





//Deallocates an existing event manager
// void destroyEventManager(EventManager em)
// {
//     if(em == NULL) 
//     {
// 		return;
// 	}
// 	EventManager em_to_delete = em;
// 	em = em->next;
// 	free(em_to_delete);    
// }

// //Compares ID of two events
// static bool eventIdCompare(const int event_ID_1, const int event_ID_2)
// {
// 	if((event_ID_1) == (event_ID_2))
// 	{
// 		return true;
// 	}
// 	return false;
// }

//Find event with the same name
// static bool eventFind_ID(const int event_id_1, EventManager em)///CHANGE TO INT!!!!!!!!
// {
// 	for (Event ptr = (em->event); ptr != NULL; ptr = ptr->next)
// 	{
// 		int new_id = ptr->event_id;
// 		if (eventIdCompare(event_id_1,new_id))
// 		{
// 			return true;
// 		}
// 	}
// 		return false;
// }

//Compares names of two events
// static bool eventNameCompare(const char* event_name_1, const char* event_name_2)
// {
// 	if(strcmp(event_name_1, event_name_2) == EQUAL)
// 	{
// 		return true;
// 	}
// 	return false;
// }


//Find event with the same name
// static bool eventFindName(const char* event_name_1, EventManager em)///
// {
// 	for (Event ptr = (em->event); ptr != NULL; ptr = ptr->next)
// 	{
// 		char* new_name = ptr->event_name;
// 		if (eventNameCompare(event_name_1,new_name))
// 		{
// 			return true;
// 		}
// 	}
// 		return false;
// }


// //Adds new event to existing date
// EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id)
// {
// 	if((em == NULL) || (event_name == NULL) || (date == NULL))
// 	{
// 		return EM_NULL_ARGUMENT;
// 	}
// 	Date current_date = em->date;
// 	if (dateCompare(date, current_date) < VALID_DATE)
// 	{
// 		return EM_INVALID_DATE;
// 	}
// 	if (event_id < VALID_ID)
// 	{
// 		return EM_INVALID_EVENT_ID;
// 	}
// 	if(eventFindName(event_name,em))
// 	{
// 		return EM_EVENT_ALREADY_EXISTS;
// 	}
// 	if(eventFind_ID(event_id, em))
// 	{
// 		return EM_EVENT_ID_ALREADY_EXISTS;
// 	}
// 	///OTHER CASES
// 	////
// 	///AT THE END SUCESS
// 	return EM_SUCCESS;
// }

// static copy_element(PQElement event)
// {
// 	Event new_event = eventCopy((Event)event);
// 	return new_event;

// }
// static bool findEventIdInEm(EventManager em, int id)
// {
// 	//for (PQElementPriority ptr = em->events; ptr != NULL; ptr = ptr->next) 
// 	//{
// 		PQ_FOREACH(PQElement, iterator, em->events)
// 		{
// 			int id_iterator = get
// 		}
// 		if(compareEventsId(ptr->event_id, id))
// 		{
// 			return true;
// 		}
// 		else
// 		{
// 			return false	
// 		}
// 	//}
// }
