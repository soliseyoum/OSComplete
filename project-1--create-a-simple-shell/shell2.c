#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

char prompt[256];
char delimiters[] = " \t\r\n";
extern char **environ;



int main() {
    // Stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];
    char cmd_bak[MAX_COMMAND_LINE_LEN];
  
    // Stores the tokenized command line input.
    char *arguments[MAX_COMMAND_LINE_ARGS];
  
    int i;
    pid_t pid;
    bool isBackground = false;
  
    while (true) {
        isBackground = false;
        do{ 
            // Print the shell prompt.
  
          	// 0. Modify the prompt to print the current working directory

            if (getcwd(prompt, sizeof(prompt)) != NULL) {
              printf("%s> ", prompt);
              fflush(stdout);
            } else {
              perror("getcwd() error");
              exit(1);
            }


            // Read input from stdin and store it in command_line. If there's an
            // error, exit immediately. (If you want to learn more about this line,
            // you can Google "man fgets")
        
            if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
                fprintf(stderr, "fgets error");
                exit(0);
            }
 
        }while(command_line[0] == 0x0A);  // while just ENTER pressed

      
        // If the user input was EOF (ctrl+d), exit the shell.
        if (feof(stdin)) {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }

        // TODO:
        // 
      
        // 1. Tokenize the command line input (split it on whitespace)
      
      i = 0;
     
  //    printf("CLI = %s\n",command_line); //eventually comment out
      arguments[0] = strtok(command_line, delimiters);
      while (arguments[i] != NULL) {
    //    printf("-%s\n", arguments[i]); //eventually comment out
        i++;
        arguments[i] = strtok(NULL, delimiters);
    }
      arguments[i] != NULL;

      
        // 2. Implement Built-In Commands
      
      if(strcmp(arguments[0],"cd") == 0){
        chdir(arguments[1]);
      }
      else if(strcmp(arguments[0],"pwd") == 0){
        printf("%s\n",prompt);
      }
      else if(strcmp(arguments[0],"echo") == 0){
        i = 1;
        while (arguments[i] != NULL) {
          if (*arguments[i] == '$') {
            printf("%s", getenv((arguments[i] + 1)));
          }
          else {
            printf("%s ", arguments[i]);       
          }
          i++;
        }
        printf("\n");
      }
      else if(strcmp(arguments[0],"exit") == 0){
        exit(0);
      }
      else if(strcmp(arguments[0],"env") == 0){
        printf("%s\n", getenv(arguments[1]));
      }
      else if(strcmp(arguments[0],"setenv") == 0){
        printf("do setenv"); 
      }
      else {
        if(strcmp(arguments[i-1],"&") == 0){
          arguments[i-1] = NULL;
          isBackground = true; //flag
        }
                // 3. Create a child process which will execute the command line input
//. 1:17:30 in lecture
        pid = fork();
        if (pid < 0) {
          perror("fork failed");
          exit(0);
        }
        else if(pid==0) { //child
          execvp(arguments[0], arguments);
          
          exit(0);
        }
        else { //parent
        // 4. The parent process should wait for the child to complete unless its a background process
        if(isBackground != true){
          wait(NULL);
        }
       }
      }
          
      
        // Hints (put these into Google):
        // man fork
        // man execvp
        // man wait
        // man strtok
        // man environ
        // man signals
        
        // Extra Credit
        // man dup2
        // man open
        // man pipes
    }
    // This should never be reached.
    return -1;
}