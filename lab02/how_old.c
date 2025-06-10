#include <stdio.h>

int main(int argc, char *argv[])
{
    int current_year = time(NULL);
    int birth_year;
    int had_birthday; /* used as boolean */

    /* TODO: ask what year the user was born in */
    printf("When you was born (year): ");
    scanf("%d", &birth_year);

    /* TODO: ask whether the user has had their birthday yet this year */
    printf("Is it already was in this year (0/1): ");
    scanf("%d", &had_birthday);

    /* TODO: compute and display the user's age */
    if (had_birthday == 0)
    {
        printf("Your age is %d\n", current_year - birth_year - 1);
    }
    else if (had_birthday == 1)
    {
        printf("Your age is %d\n", current_year - birth_year);
    }
    else
    {
        printf("\nERRROR incorrect number\n");
    }

    return 0;
}
