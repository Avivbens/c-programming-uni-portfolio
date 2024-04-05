#include <stdio.h>

int second(void);

int main(void)
{
    int testInteger;
    /* int a = 0; */
    second();
    printf("In main\n");
    printf("Enter an integer: ");

    scanf("%d", &testInteger);
    printf("Number = %d", testInteger);
    return 0;
}
