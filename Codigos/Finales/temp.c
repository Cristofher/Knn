#include <stdio.h>

int main ()
{
   FILE *fp;

   fp = tmpfile();
   printf("Temporary file created\n");

   /* you can use tmp file here */

   fclose(fp);

   return(0);
}