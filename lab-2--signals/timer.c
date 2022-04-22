#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

/* corrected code: old code was not printing correct runtime because I was
i was using cpu clock ticks, and was also redefining  begin and end
variables within term_handler and main
*/

//clock_t begin = clock();
int alarm_counter = 0; //counts times alarm has been signaled
//clock_t begin, end; // variables for begin & end time
time_t begin, end; // variables for begin & end time

void alarm_handler(int signum)
{ //signal handler
    alarm_counter = alarm_counter + 1; //increment to keep track of number of alarms
    printf("Hello World!\n");
    alarm(1); //set another alarm to go off in 1 second
}

void term_handler(int signum)
{ //signal handler
  end = time(NULL); // gets the current time when i hit 'ctrl + c'
  //double time_spent = (double)(end - begin) / CLOCKS_PER_SEC; //calculate time spent
  time_t time_spent = end - begin; //calculate time spent

  printf("\nProgram was executed for %d seconds", (int)time_spent);
  printf("\nAlarm occured %d times\n", alarm_counter);
  exit(1);
}

int main(int argc, char * argv[])
{
  begin = time(NULL); // execution time start
  signal(SIGALRM,alarm_handler); //register alarm_handler to handle SIGALRM
  signal(SIGINT, term_handler); // register term_handler to handle SIGINT
  alarm(1); //Schedule a SIGALRM for 1 second
  
  while(alarm(1)){
    pause();
    printf("Turing was right!\n");
  } 
  return 0; //never reached
}