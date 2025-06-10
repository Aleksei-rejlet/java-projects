#include <stdio.h>

int main(int argc, char *argv[])
{
    int i;
    printf("enter the int: ");
    scanf("%d", &i);
    /* TODO: read integer as an int into i */

    int smallest = i;
    int largest = i;

    /* TODO: loop forever until '0' is typed in */
    while (i != 0)
    {
        /* TODO: if new number is smaller than smallest, update smallest */
        if (i < smallest)
        {
            smallest = i;
        }

        /* TODO: if new number is larger than largest, update largest */
        if (i > largest)
        {
            largest = i;
        }

        /* TODO: read the next number as an int into i */
        printf("next one:");
        scanf("%d", &i);
    }

    printf("smallest: %d, largest: %d\n", smallest, largest);

    return 0;
}
