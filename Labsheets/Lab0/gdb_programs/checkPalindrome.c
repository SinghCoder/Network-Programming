#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
char *strrev(char *str)
{
      char *p1, *p2;
 
      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) ; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}
int palindrome(char *string)
{
   char *string2;
   int  rc;
 
   /* Duplicate string for comparison                                         */
 
   if (NULL == (string2 = strdup(string))) {
      printf("Storage could not be reserved for string\n");
      exit(EXIT_FAILURE);
   }
 
   /* If result equals 0, the string is a palindrome                          */
 
   rc = strcasecmp(string, strrev(string2));
   free(string2);
   return rc;
}
 
int main(void)
{
   char string[100];
 
   printf("Please enter a string.\n");
   scanf("%s", string);
   if (palindrome(string))
      printf("The string is not a palindrome.\n");
   else
      printf("The string is a palindrome.\n");
   return 0;
 
   
}