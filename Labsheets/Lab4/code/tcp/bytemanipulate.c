 /* Demonstrating memset(), memcpy(), and memmove(). */

#include <stdio.h>
#include <string.h>

char message1[60] = "One way to remember the order of the two pointers";
char message2[60] = "memcmp compares two arbitrary byte strings and returs 0 if they are indetical";
char temp[60];

main ()
{
  printf ("\nmessage1[] before memset():\t%s", message1);
  memset (message1 + 5, '@', 10);
  printf ("\nmessage1[] after memset():\t%s", message1);

  strcpy (temp, message2);
  printf ("\n\nOriginal message: %s", temp);
  memcpy (temp + 4, temp + 16, 10);
  printf ("\nAfter memcpy() without overlap:\t%s", temp);
  strcpy (temp, message2);
  memcpy (temp + 6, temp + 4, 10);
  printf ("\nAfter memcpy() with overlap:\t%s", temp);

  strcpy (temp, message2);
  printf ("\n\nOriginal message: %s", temp);
  memmove (temp + 4, temp + 16, 10);
  printf ("\nAfter memmove() without overlap:\t%s", temp);
  strcpy (temp, message2);
  memmove (temp + 6, temp + 4, 10);
  printf ("\nAfter memmove() with overlap:\t%s\n", temp);

}
