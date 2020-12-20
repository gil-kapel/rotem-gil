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
#define INVALID -1
#define SUBTRUCT -1
#define RAISE 1

struct EventManager_t 
{
	PriorityQueue events;
	PriorityQueue members;
	Date first_date;
};

//events functions:

static PQElement copyEventInEm(PQElement elem)
{
	if(elem == NULL)
	{
		return NULL;
	}
	return eventCopy(elem);
}

static void freeEventInEm(PQElement elem)
{
	if(elem)
	{
		eventDestroy(elem);
	}
}

static bool equalEventInEm(PQElement elem1,PQElement elem2)
{
	return eventCompare(elem1,elem2);
}

static int emCompareDate(PQElementPriority priority1, PQElementPriority priority2)
{
	return eventDateCompare(priority1, priority2);
} 

//members functions:

static PQElement copymemberInEm(PQElement elem)
{
	if(elem == NULL)
	{
		return NULL;
	}
	return memberCopy(elem);
}

static void freeMemberInem(PQElement elem)
{
	if(elem)
	{
		memberDestroy(elem);
	}
}

static bool equalMembersInEm(PQElement elem1,PQElement elem2)
{
	return memberCompare((void*)elem1, (void*)elem2);
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
	em->events = pqCreate(copyEventInEm, freeEventInEm, equalEventInEm, copyEventInEm, freeEventInEm, emCompareDate);
	if(!(em->events))
	{
		return NULL;
	}
	em->members = pqCreate(copymemberInEm,freeMemberInem,equalMembersInEm,copymemberInEm,freeMemberInem,compareMemberAmount);
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

static bool isEventFromId(EventManager em, int id)
{
	PQ_FOREACH(Event, current, em->events)
	{
		if ((getEventId(current)) == id)
		{
			return true;
		}
	}
	return false;
}

static bool eventNameinSameDate(EventManager em, char* event_name, Date date)
{
	PQ_FOREACH(Event, current, em->events)
	{
		if (!dateCompare(date ,getEventdate(current))) 
		{
			if(!strcmp(event_name, getEventName(current)))
			{
				return true;
			}
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
	PriorityQueueResult res = pqInsert(em->events, new_event, new_event);
	eventDestroy(new_event);
	if(res == PQ_OUT_OF_MEMORY)
	{
		destroyEventManager(em);
		return EM_OUT_OF_MEMORY;
	}
	return EM_SUCCESS;
}

static Date tickDateByDays(Date date, int days)
{
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
	tickDateByDays(date, days);
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
	PQ_FOREACH(Member, current, getMembersPerEvent(event_to_remove))
	{
		Member new_member = memberAmountChange(current, SUBTRUCT);
		PriorityQueueResult res1 = pqChangePriority(em->members, new_member, current, new_member);
		memberDestroy(new_member);
		if(res1 == PQ_OUT_OF_MEMORY)
		{
			destroyEventManager(em);
			return EM_OUT_OF_MEMORY;
		}
	}
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
	Event old_event = eventCopy(getEventFromId(em, event_id));
	Event new_event = eventDateChange(getEventFromId(em, event_id), new_date);
	if(eventNameinSameDate(em, getNameFromEvent(new_event), new_date))
	{
		eventDestroy(old_event);
		eventDestroy(new_event);
		return EM_EVENT_ALREADY_EXISTS;	
	}
	PriorityQueueResult res = pqChangePriority(em->events, new_event, old_event, new_event);
	eventDestroy(old_event);
	eventDestroy(new_event);
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
	{
		if (getMemberId(current) == member_id)
		{
			return true;
		}
	}
	return false;
}

static Member getMemberById(EventManager em, int member_id)
{
	PQ_FOREACH(Member, current, em->members)
	{
		if (getMemberId(current) == member_id)
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
	PriorityQueueResult res = pqInsert(em->members, new_member, new_member);
	memberDestroy(new_member);
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
	PQ_FOREACH(Member, iterator, getMembersPerEvent(chosen_event))//can we point on a queue and than on an elem inside the queue
	{
		if (getMemberId(iterator) == member_id)
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
	if(!isEventFromId(em, event_id))
	{
		return EM_EVENT_ID_NOT_EXISTS;
	}
	if(!findMemberById(em, member_id))
	{
		return EM_MEMBER_ID_NOT_EXISTS;
	}
	if(eventWithSameMember(em, member_id, event_id))
	{
		return EM_EVENT_AND_MEMBER_ALREADY_LINKED;
	}
	Member old_member = memberCopy(getMemberById(em, member_id));
	Member new_member =  memberAmountChange(old_member, RAISE);
	Event current_event = getEventFromId(em, event_id);
	PriorityQueueResult res2 = pqInsert(getMembersPerEvent(current_event), new_member, new_member);
	if(res2 == PQ_OUT_OF_MEMORY)
	{
		memberDestroy(old_member);
		memberDestroy(new_member);
		destroyEventManager(em);
		return EM_OUT_OF_MEMORY;
	}
	PriorityQueueResult res1 = pqChangePriority(em->members, new_member, old_member, new_member);
	memberDestroy(old_member);
	memberDestroy(new_member);
	if(res1 == PQ_OUT_OF_MEMORY)
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
	if(!isEventFromId(em, event_id))
	{
		return EM_EVENT_ID_NOT_EXISTS;
	}
	if(!findMemberById(em, member_id))
	{
		return EM_MEMBER_ID_NOT_EXISTS;
	}
	if(!eventWithSameMember(em, member_id, event_id))
	{
		return EM_EVENT_AND_MEMBER_NOT_LINKED;
	}
	Member old_member = memberCopy(getMemberById(em, member_id));
	Member new_member = memberAmountChange(old_member, SUBTRUCT);
	PriorityQueueResult res1 = pqChangePriority(em->members, new_member, old_member, new_member);
	if(res1 == PQ_OUT_OF_MEMORY)
	{
		memberDestroy(old_member);
		memberDestroy(new_member);
		destroyEventManager(em);
		return EM_OUT_OF_MEMORY;
	}
	PriorityQueueResult res2 = pqRemoveElement(getMembersPerEvent(getEventFromId(em, event_id)), old_member);
	memberDestroy(old_member);
	memberDestroy(new_member);
	if (res2 == PQ_OUT_OF_MEMORY)
	{
		destroyEventManager(em);
		return EM_OUT_OF_MEMORY;
	}

	return EM_SUCCESS;
}

static EventManagerResult remove_prev_events(EventManager em)
{
	Event first_event = pqGetFirst(em->events);
	while(dateCompare(getEventdate(first_event), em->first_date) < 0)
	{
		Member first_member = pqGetFirst(getMembersPerEvent(first_event));
		while(first_member)
		{
			EventManagerResult res1 = emRemoveMemberFromEvent(em, getMemberId(first_member), getEventId(first_event));
			if(res1 != EM_SUCCESS)
			{
				return EM_OUT_OF_MEMORY;
			}
			first_member = pqGetFirst(getMembersPerEvent(first_event));
		}
		PriorityQueueResult res2 = pqRemove(em->events);
		if(res2 == PQ_NULL_ARGUMENT)
		{
			return EM_OUT_OF_MEMORY;
		}
		first_event = pqGetFirst(em->events);
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
	tickDateByDays(em->first_date, days);
	return remove_prev_events(em);
}

int emGetEventsAmount(EventManager em)
{
	if(em == NULL)
	{
		return INVALID;
	}
	return pqGetSize(em->events);
}

char* emGetNextEvent(EventManager em)
{
	if(em == NULL || em->events == NULL)
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
		fprintf(fp,",%s",getMemberName(current));

	}
}

static void printCurrentEvent(Event event, FILE* fp)
{
	fprintf(fp,"%s", getEventName(event));
	Date date = getEventdate(event);
	int* day = malloc(sizeof(int));
	int* month = malloc(sizeof(int));
	int* year = malloc(sizeof(int));
	dateGet(date,day,month,year);
	fprintf(fp, ",%d.%d.%d",*day, *month,*year);
	free(day);
	free(month);
	free(year);
	printMembersPerEvent(event, fp);
}

void emPrintAllEvents(EventManager em, const char* file_name)
{
	FILE* fp = fopen(file_name, "w");
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
	fprintf(fp,",%d", getMemberAmount(member));
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
		if(getMemberAmount(current) > 0)
		{
			printCurrentMember(current, fp);
			fprintf(fp, "\n");// is 'a' needed again?
		}
	}
	fclose(fp);	
}