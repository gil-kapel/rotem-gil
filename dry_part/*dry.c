#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#define SIZE 1000 

char* stringDuplicator(char* string, int times)
{
    if((string) && (times > 0))
    {
        int length = strlen(string);
        char* out = malloc(sizeof(char)*(length*times)+1);
        if(!out)
        {
            printf("wrong out!\n");
            return NULL;
        }
        char* out_temp = out;
        for (int i=0; i < times; i++)
        {
            strcpy(out_temp,string);
            out_temp += length;
        }
        *(out_temp) = '\0';
        return  out;
    }
    else
    {
        return NULL;   
    }
}



int main(){
    char *string = malloc(SIZE);
    int times = 0;
    char *duplicate = {NULL};
    printf("Enter a string and number of times:\n");
    scanf("%s %d", string, &times);
    if(!(stringDuplicator(string,times)))
    {
        return 0;
    }
    duplicate = stringDuplicator(string,times);
    printf("%s\n",duplicate);
    free(string);
    free(duplicate);
    return 0;
}
