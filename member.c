#include "member.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event_manager.h"
#include "priority_queue.h"
#include "date.h"
#include "event.h"
#define INIT_AMOUNT 0


struct Member_t
{
	char* member_name;
	int* member_id;
    //int id;
	int* amount;
};

static char* stringCopy(char* new_str)
{
    if(!new_str)
    {
        return NULL;
    }
    char* copy_str = malloc(strlen(new_str)+1);
    return (copy_str == NULL) ? strcpy(copy_str, new_str) : NULL;
}

Member memberCreate(char* name, int* id)//, int* amount)//, int amount)//,PriorityQueue members_per_event);
{
    Member member = malloc(sizeof(*member));
    if (member == NULL) 
    {
        return NULL;
    }
	member->member_name = stringCopy(name);
    if (member->member_name == NULL) 
    {
        memberDestroy(member);
        return NULL;
    }
    int* member_id = malloc(sizeof(int));
    if (member_id == NULL)
    {
        memberDestroy(member);
        return NULL;
    }
    *(member->member_id) = *id;
    free(id);
    int* amount = malloc(sizeof(int));
    if (amount == NULL)
    {
        return NULL;
    }
    *(member->amount) = INIT_AMOUNT;
    //member->amount = amount;
	return member;
}

Member memberCopy(Member member)
{
    if(member == NULL)
    {
        return NULL;
    }
    return memberCreate(member->member_name, member->member_id);//, member->amount);
}

//Deallocates an existing Date
void memberDestroy(Member member)
{
    if(member == NULL)
    {
        return;
    }
    free(member->amount);
    free(member->member_id);
    free(member->member_name);
    free(member);
}

bool memberCompare(Member member1, Member member2)
{
    if((member1 == NULL) || (member2 == NULL))
    {
        return NULL;
    }
    int name_res = strcmp(member1->member_name,member2->member_name);
    int id_res = ((member1->member_id) - (member2->member_id));
    //int amount_res = ((member1->amount) - (member2->amount));
    if((!name_res) && (!id_res))// && (!amount_res))
    {
        return true;
    }
    return false;
}

int* getMemberId(Member member)
{
    if(member == NULL)
    {
        return NULL;
    }
    return member->member_id;
}

char* getMemberName(Member member)//
{
    if(member == NULL)
    {
        return NULL;
    }
    return member->member_name;
}

int* copyMemberId(int* member_id)
{
    if(member_id == NULL)
    {
        return NULL;
    }
    int* new_member_id = malloc(sizeof(int));   
    if(new_member_id == NULL)
    {
        free(new_member_id);
        return NULL;
    }
    *new_member_id = *member_id;
    return new_member_id;
} 

void freeMemberId(int* member_id)
{
    free(member_id);
}
// int getMemberAmount(Member member)
// {
//     return member->amount;
// }

int memberIdCompare(int* member_id1, int* member_id2)
{
    // if((member_id1 == NULL) || (member_id2 == NULL))
    // {
    //     return NULL;
    // } 
    int first_id = *member_id1;
    int second_id = *member_id2;
    return (first_id - second_id);
}


char* getNameFromMember(Member member)
{
    if(member == NULL)
    {
        return NULL;
    }
    return stringCopy(member->member_name);
}

int* copyAmount(int* amount)
{
    if(amount == NULL)
    {
        return NULL;
    }
    int* new_amount = malloc(sizeof(int));   
    if(new_amount == NULL)
    {
        free(new_amount);
        return NULL;
    }
    *new_amount = *amount;
    return new_amount;
} 

void amountDestroy(int* amount)
{
    free(amount);
}


int amountCompare(Member member1, Member member2)
{
    int amount_delta = (getMemberAmount(member1)) - (getMemberAmount(member2));
    if(!amount_delta)
    {
        return amount_delta;
    }
    return (getMemberId(member1)-getMemberId(member2));
}

int* getMemberAmount(Member member)
{
    return member->amount;
}
