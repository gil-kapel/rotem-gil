#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include <stdio.h>
#define SIZE 1000 

char* stringDuplicator(char* string, int times){
    assert(string);
    assert(times > 0);
    int length = strlen(string);
    char* out = malloc(length*times+1);
    assert(out);
    for (int i=0; i<times; i++){
        strcpy(out,string);
        out += length;
    }
    *out = '\0';
    return out - length*times;
}



int main(){
    char *string = malloc(SIZE);
    int times;
    char *duplicate = {NULL};
    printf("Enter a string and number of times:\n");
    scanf("%s %d", string, &times);
    duplicate = stringDuplicator(string,times);
    printf("new string is: %s \n",duplicate);
    free(string);
    return 0;
}
