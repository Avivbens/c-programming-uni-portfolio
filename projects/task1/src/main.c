#include <stdio.h>
#include "./header.h"

int main(void)
{
    int testInteger;
    /* int a = 0; */
    second(1234);
    printf("In main\n");
    printf("Enter an integer: ");

    scanf("%d", &testInteger);
    printf("Number = %d", testInteger);
    return 0;
}
