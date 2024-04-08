#include <stdio.h>

#include "./second/second.h"

int main(void) {
    int testInteger;
    /* int a = 0; */
    second(12364);
    printf("In main\n");
    printf("Enter an integer: ");

    scanf("%d", &testInteger);
    printf("Number = %d", testInteger);
    return 0;
}
