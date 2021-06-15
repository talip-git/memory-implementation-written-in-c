#include <stdio.h>
#include "mem.c"
int main(){
    start(100000);
    // testing our functions
    int status;
    char *s = (char*)TEDU_alloc(20*sizeof(char));
    char *a = (char*)TEDU_alloc(100*sizeof(char));
    char *b = (char*)TEDU_alloc(100*sizeof(char));
    int *array = (int*) TEDU_alloc(24*sizeof(int));
    status = TEDU_GetStats();
    if (status == 0){
        printf("Cannot print the stats!(Check your memory!)\n)");
    }
    TEDU_Free(s);
    TEDU_Free(a);
    TEDU_Free(b);
    TEDU_Free(array);
    status = TEDU_GetStats();
    printf("****************************************************\n");
    printf("Trying to print the stats after Free from memory...\n");
    if (status == 0){
        printf("Cannot print the stats!(Check your memory!)\nYou can not call stats method if you used TEDU_Free() method!\n");
    };

    return 0;
}

