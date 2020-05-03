#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <math.h>

void func(void);
jmp_buf place;
double square1(double n2){

                if(n2<=0) longjmp(place, 2);
                return n2*n2;
                }
        double  compute(int n){

                if (n<=0) longjmp(place, 1);
                double n1=sqrt((double)n);
                double n2=square1(n1);
                return(n2-n);

}


main(){
        int retval;
        int i=1, n;
        /*
         * First call returns 0,
         * a later longjmp will return non-zero.
         */
        if((retval=setjmp(place)) != 0){
          printf("Returned using longjmp\n");
          printf("Ret value: %d", retval);
          }

        while(i<100){
                printf("Enter value %d: ", i);
                scanf("%d", &n);
                double n3=compute(n);
                printf("Result=%ld\n", n3);
                i++;
                }
        }
