#include "stdlib.h"
#include "string.h"
#include "assert.h"

void stringDuplicator(char* string, int times){
    assert(string != NULL && times > 0);
    int length = strlen(string);
    char* out = malloc(length*times);
    assert(out);
    for (int i=0; i<times; i++){
        strcpy(out,string);
        out += length;
    }
    out = out - length*times;
    strcpy(string, out);
    free(out);
}

#include <stdio.h>

int main(){
    char *string = malloc(1000);
    int times;
    printf("Enter a string and number of times:\n");
    scanf("%s %d", string, &times);
    stringDuplicator(string,times);
    printf("new string is: %s \n",string);
    free(string);
    return 0;
}