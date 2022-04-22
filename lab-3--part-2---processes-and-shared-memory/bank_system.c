#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <unistd.h>
#include  <sys/wait.h>
#include  <time.h> 

void  Deposit(int* BankAccount);
void  Withdraw(int* BankAccount);

int  main(int  argc, char *argv[]) {
	int ShmID;
	int *ShmPTR;
	pid_t  pid;
	int* BankAccount;
	int* Turn;

	ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
	if (ShmID < 0) {
		printf("*** shmget error ***\n");
		exit(1);
	}
	printf("Process has recieved shared memory of two integers...\n");

	ShmPTR = (int *) shmat(ShmID, NULL, 0);

	if (*ShmPTR == -1) {
		printf("*** shmat error (server) ***\n");
		exit(1);
	}
	printf("Process has attached the shared memory...\n");

	ShmPTR[0] = 0;
	ShmPTR[1] = 0;
	
	BankAccount = &ShmPTR[0];
	Turn = &ShmPTR[1];

	//srand(time(0));
    srand(getpid());

    printf("Server is about to fork a child process...\n");
	pid = fork();
	if (pid > 0) { // parent process
    // Prent's turn is 0

		    for(int i = 0 ; i < 25 ; i++){ //loop this 25 times
			sleep(rand() % 6); //sleep 0-5 seconds
            printf(" Parent loop: %d\n", i);

			while (*Turn != 0) {
                ;
            }
			if (*BankAccount <= 100) { // try to deposit money if accounr is under 100
				Deposit(BankAccount);
			} else { //if student account is over 100, child has enough money
                printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", *BankAccount);
			}
			*Turn = 1; //set Turn = 1
		}
			wait(&pid); //wait until child
			shmdt((void *) ShmPTR); //detach mem
			shmctl(ShmID, IPC_RMID, NULL); // remove process shared mem and exit
			exit(0);
	}else {
        //child process
    // child's turn is 1

		    for(int i = 0 ; i < 25 ; i++){ //loop this 25 times
			sleep(rand() % 6); // sleep 0-5 seconds
			printf(" Child loop: %d\n", i);
			while (*Turn != 1) { //while Turn is not child's no op
                ;
            }
			Withdraw(BankAccount); //child proc
			*Turn = 0; // change turn to 0
		}

		exit(0);
	}
}

void Deposit(int* BankAccount) {
	int balance = (rand() % 101); //set random balanc 0-100
	if ((balance % 2) == 0) { //if balance even, deposit it
		*BankAccount += balance;
		printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, *BankAccount);
	} else { //if odd, not nough money
		printf("Dear old Dad: Doesn't have any money to give\n");
	}
}

void Withdraw(int* BankAccount) {
	int balance = (rand() % 51); //balance btnw 0-50
	printf("Poor Student needs $%d\n", balance);
	if (balance <= *BankAccount) { //f there's enough money
		*BankAccount -= balance;
		printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, *BankAccount);
	} else { //if there isn't enough money
		printf("Poor Student: Not Enough Cash ($%d)\n", *BankAccount);
	}
}
