#ifndef MEMBER_H_
#define MEMBER_H_

#include <stdbool.h>

typedef struct Member_t *Member;

Member memberCreate(char* name, int* id);//, int* amount);//, int amount);//,PriorityQueue members_per_event);

Member memberCopy(Member member);

//Deallocates an existing Member
void memberDestroy(Member member);

bool memberCompare(Member member1, Member member2);

int* getMemberId(Member member);

char* getMemberName(Member member);

// int getMemberAmount(Member member);

char* getNameFromMember(Member member);

int* copyMemberId(int* member_id);

void freeMemberId(int* member_id);

int* copyAmount(int* amount);

void amountDestroy(int* amount);

int amountCompare(Member member1, Member member2);

int memberIdCompare(int* member_id1, int* member_id2);

int* getMemberAmount(Member member);

Member memberAmountChange(Member member, int change);

#endif //MEMBER_H_
