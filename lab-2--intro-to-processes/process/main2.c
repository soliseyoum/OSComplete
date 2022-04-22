#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

 /* corrected code: MAX_COUNT was defined incorrectly.
 random number ranges started at 0 and were right-exclusive
 
 */

//#define   MAX_COUNT  random() % 30+1
#define   MAX_COUNT  200

void  ChildProcess(void);                /* child process prototype  */
void  ParentProcess(void);               /* parent process prototype */


void  main(void)
{
     pid_t  child_a, child_b;

     child_a = fork();
     if (child_a == 0) // if in child process A
       ChildProcess();
     else if (child_a < 0) //else if fork was unsuccessful, exit failure
       exit(1);
     else
       child_b = fork();
       if (child_b == 0) // if in child process B
         ChildProcess();
       else if (child_b < 0) //else if fork was unsuccessful, exit failure
         exit(1);
       else
         ParentProcess(); //else, in parent process        
}


void  ChildProcess(void)
{
  int   i;
  time_t t;
  /* Intializes random number generator */
  srandom((unsigned) time(&t));
  pid_t pid = getpid();
  pid_t ppid = getppid();
  for (i = 1; i <= ((random() % 30)+1); i++) {
    printf("Child Pid: %d is going to sleep!\n", pid);
    sleep(((random() % 10)+1));
    printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n", pid, ppid);
    }
    exit(0);
}


void  ParentProcess(void)
{
  int   i, status;
  pid_t wpid;
  
  while ((wpid = wait(&status)) > 0){
    printf("Child Pid: %d has completed\n", (int)wpid);
    }

}