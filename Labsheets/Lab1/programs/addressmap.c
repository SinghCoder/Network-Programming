

#include <stdlib.h>
#include <stdio.h>
double t[0x02000000];
void segments()
{
static int s = 42;
void *p = malloc(1024);
printf("stack\t%010p\nbrk\t%010p\nheap\t%010p\n"
"static(bss)\t%010p\nstatic(data)\t%010p\ntext\t%010p\n",
&p, sbrk(0), p, t, &s, segments);
}
int main(int argc, char *argv[])
{
segments();
exit(0);
}
