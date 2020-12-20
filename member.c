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
#define EQUAL 0
#define NOT_RESPONSIBLE 0
struct Member_t
{
	char* member_name;
	int member_id;
	int amount;
};

Member memberCreate(char* name, int id)
{
    if(name == NULL)
    {
        return NULL;
    }
    Member member = malloc(sizeof(*member));
    if (member == NULL) 
    {
        return NULL;
    }
    member->member_name = malloc(strlen(name) + 1);
    if (member->member_name == NULL) 
    {
        return NULL;
    }
    strcpy(member->member_name, name);
    member->member_id = id;
    member->amount = NOT_RESPONSIBLE;
	return member;
}

Member memberCopy(Member member)
{
    if(member == NULL)
    {
        return NULL;
    }
    Member member_copy = memberCreate(member->member_name, member->member_id);
    member_copy->amount = member->amount;
    return member_copy;
}

void memberDestroy(Member member)
{
    if(member)
    {
        free(member->member_name);
        free(member);
    }
}

bool memberCompare(Member member1, Member member2)
{
    if((member1 == NULL) || (member2 == NULL))
    {
        return NULL;
    }
    if(member1->member_id - member2->member_id == EQUAL)
    {
        return true;
    }
    return false;
}

int getMemberId(Member member)
{
    return member->member_id;
}

char* getMemberName(Member member)
{
    if(member == NULL)
    {
        return NULL;
    }
    return member->member_name;
}

int memberIdCompare(Member member1, Member member2)
{
    return member2->member_id - member1->member_id;
}

char* getNameFromMember(Member member)
{
    if(member == NULL)
    {
        return NULL;
    }
    return member->member_name;
}

int amountCompare(Member member1, Member member2)
{
    int amount_delta = getMemberAmount(member1) - getMemberAmount(member2);
    if(amount_delta)
    {
        return amount_delta;
    }
    return getMemberId(member2)-getMemberId(member1);
}

int getMemberAmount(Member member)
{
    return member->amount;
}

Member memberAmountChange(Member member, int change)
{
    if(member == NULL)
    {
        return NULL;
    }
    Member new_member = memberCopy(member);
    if(change > 0)
    {
        new_member->amount = member->amount + 1;
    }
    else if(change < 0)
    {
        new_member->amount = member->amount - 1;
    }
    return new_member;
}
