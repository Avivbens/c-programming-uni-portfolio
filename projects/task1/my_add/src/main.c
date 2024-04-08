#include <stdio.h>

#include "./second/second.h"

int main(void) {
    int testInteger;
    second(12364);
    printf("In main\n");
    printf("Enter an integer: ");
    int a = 0;

    scanf("%d", &testInteger);
    printf("Number = %d", testInteger);
    return 0;
}
