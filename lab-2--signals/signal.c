/* hello_signal.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>


/* corrected code: old code was not executing indefinitely because 
I used sleep(1) instead of pause() in main
*/


void handler(int signum)
{ //signal handler
  printf("Hello World!\n");
  alarm(1); 
}

int main(int argc, char * argv[])
{
  signal(SIGALRM,handler); //register handler to handle SIGALRM
  alarm(1); //Schedule a SIGALRM for 1 second
  while(alarm(1)){
    pause();
    printf("Turing was right!\n");
  } 
  return 0; //never reached
}