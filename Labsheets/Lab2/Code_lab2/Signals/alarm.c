#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int n; 

void sigalrm(int signo){
    alarm(1);
    n=n+1;
    printf("%d seconds elapsed\n",n);
}

int main(){
    signal(SIGALRM, sigalrm);
    alarm(1);
    while(1) pause();
}