#include <stdio.h>
#include <stdlib.h>

void append(FILE *fin, FILE *fout)
{
  char ch;

  /* step 1: get the next character from the file fin */
  ch = fgetc(fin);
  /* step 2: repeat until you come across the EOF char */
  while (ch != EOF)
  {
    /* step 3: copy each character from fin into file fout */
    fputc(ch, fout);
    ch = fgetc(fin);
  }
}

int main(int argc, char **argv)
{
  FILE *fin, *fout;
  int i;

  /* step 1: check that argc is 3 */
  if (argc == 5)
  {
    /* step 2: get filename from the 1st command argument, open file */
    fout = fopen(argv[argc - 1], "w");
    /* step 3: check that the file was read, where fin == NULL */
    if (fin != NULL)
    {
      /* For the command:

         ./append file1.txt file2.txt file3.txt  out.txt

         argc == 5
         argv[0] == "append"
         argv[1] == "file1.txt"
         argv[2] == "file2.txt"
         argv[3] == "file3.txt"
         argv[4] == "out.txt"
      */

      /* step 5: loop over input file names, append it to output file */
      for (int i = 1; i < 4; ++i)
      {
        fin = fopen(argv[i], "r");
        if (fin != NULL)
        {
          append(fin, fout);
          fclose(fin);
        }
      }
      /* step 6: close the file */
      fclose(fout);
    }
  }
}
