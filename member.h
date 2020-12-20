#ifndef MEMBER_H_
#define MEMBER_H_

#include <stdbool.h>

typedef struct Member_t *Member;

Member memberCreate(char* name, int id);

Member memberCopy(Member member);

//Deallocates an existing Member
void memberDestroy(Member member);

bool memberCompare(Member member1, Member member2);

int getMemberId(Member member);

char* getMemberName(Member member);

int memberIdCompare(Member member1, Member member2);

// int getMemberAmount(Member member);

char* getNameFromMember(Member member);

int amountCompare(Member member1, Member member2);

int getMemberAmount(Member member);

Member memberAmountChange(Member member, int change);

#endif //MEMBER_H_
