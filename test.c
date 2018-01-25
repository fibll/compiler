#include <stdio.h>
#include <stdlib.h>

int main (void) {

    printf("start\n");

    int i = 0;
    int k = 0;
    long *array;

    array = (long*) malloc(sizeof(long));
    array[0] = 1;

    for(i = 0; i < 10; i++){
        printf("\n\n\n\n-----------------\nIndex: %i\n", i);

        array[0]++;
    
        printf("Calculated size: %ld", array[0] * sizeof(long));

        array = (long*) realloc(array, array[0] * sizeof(long));

        array[array[0]-1] = i;

/*
        printf("\nArray Inhalt: ");
        for(k = 0; k < array[0]; k++){
            printf("%ld", array[i+1]);
        }*/
    }



    return 0;
}