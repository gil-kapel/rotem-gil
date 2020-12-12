#ifndef EVENT_H_
#define EVENT_H_
#include <stdbool.h>
#include "date.h"

/** Type for defining the event */
typedef struct Event_t* Event;


/**
* eventCreate: Allocates a new event.
* 	NULL - if allocation failed or event is illegal.
* 	A new event in case of success.
*/
Event eventCreate(char* name, int id, Date date);

/**
* eventDestroy: Deallocates an existing event.
*/
void eventDestroy(Event event);

/**
* eventCopy: Creates a copy of target event.
*/

Event eventCopy(Event event);
/**
* eventGet: Returns the details of a event
* 	false if one of pointers is NULL.
* 	Otherwise true and the event is assigned to the pointers.
*/

// Event compareEvetsId(Event event, int id);

bool compareEventsId(int id1, int id2);

char* getEventName(Event event);

Date getEventdate(Event event);
// bool eventGet(Event event, char** name, int* id, Date date);

/**
* eventCompare: compares to events and return which comes first
*/
bool eventCompare(Event event1, Event event2);

#endif //EVENT_H_
