#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "event_manager.h"
#include "priority_queue.h"
#include "date.h"
#include "event.h"
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
	return dateCompare((void*)priority1,(void*)priority2);
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
	return memberCompare((void*)elem1, (void*)elem2);
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
	if(!(em->members))
	{
		return NULL;
	}
	em->first_date = dateCopy(date);
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
	pqDestroy(em->members);
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

static bool isEventFromId(EventManager em, int id)
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
	Date date_to_insert =  dateCopy(date);
	PQ_FOREACH(Event, current, em->events)
	if (!dateCompare(date_to_insert ,getEventdate(current))) 
	{
		if(!strcmp(event_name, getEventName(current)))
		{
			free(date_to_insert);
			return true;
		}
	}
	free(date_to_insert);
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
	if(isEventFromId(em, event_id))
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

static Date tickDateByDays(Date date, int days)
{
	//Date chosen_date = dateCopy(em->first_date);
	while (days > VALID_DATE)
	{
		dateTick(date);
		days = days - DAY;
	}
	return date;
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
	Date date = dateCopy(em->first_date);
	Date new_date = tickDateByDays(date, days);
	EventManagerResult res = emAddEventByDate(em, event_name, new_date, event_id);
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
	if(!isEventFromId(em, event_id))
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
	int* id = malloc(sizeof(int));
	*id = member_id;
	Member new_member = memberCreate(member_name, id);
	if(!new_member)
	{
		free(id);
		destroyEventManager(em);//is it really necessery
		return EM_OUT_OF_MEMORY;
	}
	int* p_member_id = malloc(sizeof(int));
	*p_member_id = member_id;
	PriorityQueueResult res = pqInsert(em->members, new_member, p_member_id);// what happened when events is NULL
	if(res == PQ_OUT_OF_MEMORY)
	{
		free(p_member_id);
		destroyEventManager(em);
		return EM_OUT_OF_MEMORY;
	}
	return EM_SUCCESS;
}


static bool eventWithSameMember(EventManager em, int member_id, int event_id)
{
	Event chosen_event = getEventFromId(em, event_id);
	PQ_FOREACH(Member, iterator, getMembersPerEvent(chosen_event))//can we point on a queue and than on an elem inside the queue
	{
		if (*(getMemberId(iterator)) == member_id)
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
	Event current_event = getEventFromId(em, event_id);
	int* m_id = malloc(sizeof(int));
	*m_id = member_id;
	PriorityQueueResult res = pqInsert(getMembersPerEvent(current_event), new_member, m_id);//is it ok to put em->events->otherqueue?
	if(res == PQ_OUT_OF_MEMORY)
	{
		freeMemberId(m_id);
		destroyEventManager(em);
		return EM_OUT_OF_MEMORY;
	}
	int* amount = getMemberAmount(new_member);
	int* newAmount = amount + 1;
	PriorityQueueResult res2 = pqChangePriority(em->members, new_member, amount, newAmount);
	if(res2 != PQ_SUCCESS)
	{
		freeMemberId(m_id);
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
	Event to_remove = getEventFromId(em, event_id);
	PriorityQueueResult res = pqRemoveElement(getMembersPerEvent(to_remove), member_to_delete);
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
	Date new_date = tickDateByDays(em->first_date, days);
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
	Event event = pqGetFirst(em->events);
	return getEventName(event);
}

static void printMembersPerEvent(Event event, FILE* fp)//print members per event
{
	PQ_FOREACH(Member, current, getMembersPerEvent(event))
	{
		fprintf(fp,",%s ",getMemberName(current));

	}
}

static void printCurrentEvent(Event event, FILE* fp)
{
	fprintf(fp,"%s ", getEventName(event));
	Date date = getEventdate(event);
	int* day = malloc(sizeof(int));
	int* month = malloc(sizeof(int));
	int* year = malloc(sizeof(int));
	dateGet(date,day,month,year);
	fprintf(fp, ",%d.%d.%d ",*day, *month,*year);
	free(day);
	free(month);
	free(year);
	printMembersPerEvent(event, fp);
}


void emPrintAllEvents(EventManager em, const char* file_name)
{
	FILE* fp = fopen(file_name, "a");
	if(!fp)
	{
		return;
	}
	PQ_FOREACH(Event, current, em->events)
	{
		printCurrentEvent(current, fp);
		fprintf(fp, "\n");// is 'a' needed again?
	}
	fclose(fp);	
}

static void printCurrentMember(Member member, FILE* fp)
{
	fprintf(fp, "%s", getMemberName(member));
	fprintf(fp,",%d ", *(getMemberAmount(member)));
}

void emPrintAllResponsibleMembers(EventManager em, const char* file_name)
{
	FILE* fp = fopen(file_name, "a");
	if(!fp)
	{
		return;
	}
	PQ_FOREACH(Member, current, em->members)
	{
		printCurrentMember(current, fp);
		fprintf(fp, "\n");// is 'a' needed again?
	}
	fclose(fp);	
}
