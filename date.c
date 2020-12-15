#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "date.h"
#define NONE 0
#define MIN_DAY 1
#define MAX_DAY 30
#define MIN_MONTH 1
#define MAX_MONTH 12
#define DAYS_IN_YEAR 365

struct Date_t
{
    int day;
    int month;
    int year;
};

//Returns if day and month are valid
static bool dateIsLegal(int day, int month) 
{
    bool day_val = (day >= MIN_DAY) && (day <= MAX_DAY);
	bool month_val = (month >= MIN_MONTH) && (month <= MAX_MONTH);
    return ((day_val) && (month_val));
}

//Allocates a new date
Date dateCreate(int day, int month, int year)
{
    if(!dateIsLegal(day, month))
    {
        return NULL;
    }
    Date date = malloc(sizeof(*date));
    if (date == NULL) 
    {
        return NULL;
    }
    date->day = day;
    date->month = month;
    date->year = year;
    return date;
}

//Deallocates an existing Date
void dateDestroy(Date date)
{
    if(date == NULL)
    {
        return;
    }
    free(date);
}

//Creates a copy of target Date
Date dateCopy(Date date)
{
    if(date == NULL)
    {
        return NULL;
    }
    return dateCreate(date->day, date->month, date->year);
}

//Returns the day, month and year of a date
bool dateGet(Date date, int* day, int* month, int* year)
{
    if((day == NULL) || (month == NULL) || (year == NULL))
    {
        return false;
    }
    *day = date->day; 
    *month = date->month; 
    *year = date->year;
    return true;
}

//Convert date to days
static int dateInDays(Date date)
{
    int date_in_days = (date->day) + ((date->month) * MAX_DAY) + ((date->year) * DAYS_IN_YEAR);
	return date_in_days;
}

//Compares to dates and return which comes first
int dateCompare(Date date1, Date date2)
{
    if((date1 == NULL) || (date2 == NULL))
    {
        return NONE;
    }
    return ((dateInDays(date1)) - (dateInDays(date2)));
}


//Increases the date by one day
void dateTick(Date date)
{
    if(date == NULL)
    {
        return;
    }
    if(((date->day) == MAX_DAY) && ((date->month) == MAX_MONTH))
    {
        date->day = MIN_DAY;
        date->month = MIN_MONTH;
        (date->year)++;
        return;
    }
    else if(((date->day) == MAX_DAY) && ((date->month) < MAX_MONTH))
    {
        date->day = MIN_DAY;
        (date->month)++;
        return;
    }
    else
    {
        (date->day)++;
    }
}





//Compares to dates and return which comes first
// int dateCompare(Date date1, Date date2)
// {
//     if((!date1) || (!date2))
//     {
//         return false;
//     }
//     if(((date1->day) == (date2->day)) && ((date1->month)==(date2->month)) && ((date1->year) == (date2->year)))
//     {
//         return EQUAL;
//     }
//     if(((date1->year) > (date2->year)) || (((date1->year) == (date2->year)) && ((date1->month) > (date2->month))) || (((date1->year) == (date2->year))
//          && ((date1->month) == (date2->month)) && ((date1->day) > (date2->day))))
//     {
//              return EARLIER;
//     }
//     return LATER;
// }



    // if (((date->day) < MAX_DAY) || ((date->month) < MAX_MONTH))
    // {
    //     (date->day)++;
    // }


// //write
// void dateTick(Date date)
// {
//     if (!date)
//     {
//         return;
//     }
//     else
//     {
        
//     }
    
    
// }




// static int dateToDays(Date date) {
// 	int month = monthToInt(date->month);
// 	return date->day + month*(MAX_DAY - MIN_DAY + 1) +
//         DAYS_IN_YEAR * date->year;
// }

// Stack stackCreate(int maxSize) {
// 	stack->array = malloc(sizeof(int)*maxSize);
// 	if (stack->array == NULL) {
// 		free(stack);
// 		return NULL;
// 	}
// 	stack->nextIndex = 0;
// 	stack->maxSize = maxSize;
// 	return stack;





//Deallocates an existing Date.
// void dateDestroy(Date date)
// {
//     //typedef struct PriorityQueue_t *PriorityQueue;
//     if (*PriorityQueue == NULL)
//     {
//         return;
//     }
//     free(date);
// }
//dateCopy: Creates a copy of target Date.
// Date dateCopy(Date date)
// {
//     if(date == NULL)
//     {
//         return NULL;
//     }
//     //strcpy(Date,date);
//     return date;
// }


 

 // static bool dateRead(Date date)
// {
// 	if (!date)
//     {
// 		return false;
// 	}
// 	if (scanf("%d %d %d", &(date->day), date->month, &(date->year)) != 3)
//     {
// 		return false;
// 	}
// 	return dateIsValid(date);
// }

