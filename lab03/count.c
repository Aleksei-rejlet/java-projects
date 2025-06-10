#include <stdio.h>
#include <stdlib.h>

/* counts how many characters are in the `fin` file */
int count(FILE *fin)
{
  int counter;
  char ch;
  counter = 0;

  /* step 1: get the next character */
  ch = fgetc(fin);
  /* step 2: loop over the file contents until the 'EOF' character is found */
  while (ch != EOF)
  {
    /* step 3: for each new character, increment the counter */
    counter++;
    ch = fgetc(fin);
  }

  return counter;
}

int main(int argc, char **argv)
{
  FILE *fin;

  /* step 1: check that argc is 3 */
  if (argc == 2)
  {
    /* step 2: get filename from the 1st command argument, open file */
    fin = fopen(argv[1], "r");
    /* step 3: check that the file was read, where fin == NULL */
    if (fin != NULL)
    {
      /* step 4: call `count` with the opened file and print its value */
      printf("%d\n", count(fin));
      /* step 5: close the file */
      fclose(fin);
    }
  }
}
