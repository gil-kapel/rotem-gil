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
	int* amount;
};

static char* stringCopy(char* new_str)
{
    if(new_str == NULL)
    {
        return NULL;
    }
    char* copy_str = malloc(strlen(new_str) + 1);
    return (copy_str != NULL) ? strcpy(copy_str, new_str) : NULL;
}

Member memberCreate(char* name, int* id)
{
    if(name == NULL || id == NULL)
    {
        return NULL;
    }
    Member member = malloc(sizeof(*member));
    if (member == NULL) 
    {
        return NULL;
    }
	member->member_name = stringCopy(name);
    if(member->member_name == NULL)
    {
		memberDestroy(member);
        return NULL;
    }
    member->member_id = copyMemberId(id);
    int* init_amount = malloc(sizeof(*init_amount));
    if(init_amount == NULL)
    {
        return NULL;
    }
    *init_amount = 0;
    member->amount = copyAmount(init_amount);
	return member;
}

Member memberCopy(Member member)
{
    if(member == NULL)
    {
        return NULL;
    }
    Member member_copy = memberCreate(member->member_name, member->member_id);
    member_copy->amount = copyAmount(member->amount);
    return member_copy;
}

//Deallocates an existing Date
void memberDestroy(Member member)
{
    if(member)
    {
        free(member->member_id);
        free(member->amount);
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
    int name_res = strcmp(member1->member_name,member2->member_name);
    int id_res = (*(member1->member_id) - *(member2->member_id));
    if((!name_res) && (!id_res))
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
    return *(member_id2) - *(member_id1);
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
    int* new_amount = malloc(sizeof(int*));
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
    int amount_delta = *(getMemberAmount(member1)) - *(getMemberAmount(member2));
    if(amount_delta)
    {
        return amount_delta;
    }
    return *(getMemberId(member2))-*(getMemberId(member1));
}

int* getMemberAmount(Member member)
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
        *(new_member->amount) = *(member->amount) + 1;
    }
    else if(change < 0)
    {
        *(new_member->amount) = *(member->amount) - 1;
    }
    return new_member;
}
