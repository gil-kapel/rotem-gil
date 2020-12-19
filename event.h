#ifndef EVENT_H_
#define EVENT_H_
#include "date.h"
#include "priority_queue.h"
#include "member.h"
#include <stdbool.h>

typedef struct Event_t *Event;

Event eventCreate(char* name, int id, Date date);//, PriorityQueue members_per_event);// change in em

//Creates a copy of target event
Event eventCopy(Event event);

//Deallocates an existing Date
void eventDestroy(Event event);

bool eventCompare(Event event1, Event event2);

int getEventId(Event event);

char* getEventName(Event event);

PriorityQueue getMembersPerEvent(Event event);

Date getEventdate(Event event);

char* getNameFromEvent(Event event);

#endif //EVENT_H_
