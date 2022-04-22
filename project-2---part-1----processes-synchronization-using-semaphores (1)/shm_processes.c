#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

void deposit(int* bankAccount){
  
  int localBalance = *bankAccount;
  
  int amount = rand() % 101;
 
  if (amount%2 == 0){
    
    localBalance+= amount;
    printf("Dear Old Dad: Deposits $%d / Balance = $%d\n", amount, localBalance);
    *bankAccount = localBalance;
  
  }else{
    printf("Dear Old Dad: Doesn't have any money to give\n");
  }
}

void withdraw(int* bankAccount){
  
  int localBalance = *bankAccount;
  
  int needs = rand() % 51;
  
  printf("Poor Student needs $%d\n", needs);
  if (needs <= localBalance){
    localBalance-=needs;
   
    printf("Poor Student: Withdraws $%d / Balance = $%d\n", needs, localBalance);
    *bankAccount = localBalance;
  
  }else{
    ("Poor Student: Not Enough Cash ($%d)\n", localBalance);
  }
}


int  main(int  argc, char *argv[])
{
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;
      sem_t *sem_mute; 
      int forward; 
      int none_0 =0; 
      int *count_ptr;
      int i;
   
    forward= open("log.txt",O_RDWR|O_CREAT,S_IRWXU);
  
  write(forward,&none_0,sizeof(int));
  
  count_ptr = mmap(NULL,sizeof(int),PROT_READ |PROT_WRITE,MAP_SHARED,forward,0);
  
  close(forward);
   
     if ((sem_mute = sem_open("semaphore", O_CREAT, 0644, 1)) == SEM_FAILED) {
   
       perror("semaphore initilization");
    exit(1);
  }

  
  pid = fork();

    if (pid > 0) {
        
      for (i = 0; i>-1; i++){
            sleep(rand()%6);
           
          printf("Dear Old Dad: Attempting to Check Balance\n");\
            sem_wait(sem_mute);
            
          int randomNumber = rand()%101;
            
          if (randomNumber % 2 == 0){
              if (*count_ptr<100){
                deposit(count_ptr);
              } else {
                printf("Dear Old Dad: Thinks Student has enough Cash ($%d)\n", *count_ptr);
              }
            
            }else{
              printf("Dear Old Dad: Last Checking Balance = $%d\n", *count_ptr);
            }
            sem_post(sem_mute);              
        }

//     
      
      
      exit(1);
    }
    else if (pid == 0) {
        for (i = 0; i>-1; i++){
            sleep(rand()%6);
            printf("Poor Student: Attempting to Check Balance\n");  
            sem_wait(sem_mute);
            int randomNumber = rand();
            if (randomNumber%2 == 0){
              withdraw(count_ptr);
            }else{
              printf("Poor Student: Last Checking Balance = $%d\n", *count_ptr);
            }
            sem_post(sem_mute);
            
        }
        printf("   Client is about to exit\n");
        exit(0);
    }

     wait(&status);
     printf("Server has detected the completion of its child...\n");
     shmdt((void *) ShmPTR);
     printf("Server has detached its shared memory...\n");
     shmctl(ShmID, IPC_RMID, NULL);
     printf("Server has removed its shared memory...\n");
     printf("Server exits...\n");
     exit(0);
}

void  ClientProcess(int  SharedMem[])
{
     printf("   Client process started\n");
     printf("   Client found %d %d %d %d in shared memory\n",
                SharedMem[0], SharedMem[1], SharedMem[2], SharedMem[3]);
     printf("   Client is about to exit\n");
}