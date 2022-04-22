#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

char prompt[256];
char delimiters[] = " \t\r\n";
extern char **environ;

char *getcwd(char *buf, size_t size);

char cwd[100];
void sighandler();

pid_t pid;

void shell_handler(int signum){ kill(pid,SIGKILL);}
void shell_timer(int signum){	kill(pid,SIGKILL);}

int main() {
    signal(SIGINT, sighandler);
  
    // Stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];
    char cmd_bak[MAX_COMMAND_LINE_LEN];
  
    // Stores the tokenized command line input.
    char *arguments[MAX_COMMAND_LINE_ARGS];
  
    int i;
    bool isBackground = false;
  
    while (true) {
        isBackground = false;
        do{ 
            // Print the shell prompt.
  
          	// 0. Modify the prompt to print the current working directory

            if (getcwd(prompt, sizeof(prompt)) != NULL) {
            //  printf("%s> ", prompt);
              //fflush(stdout);
            } else {
              perror("getcwd() error");
              exit(1);
            }
            printf("%s> ", prompt);
            fflush(stdout);

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
    //  arguments[i] != NULL;

      
        // 2. Implement Built-In Commands
      
char* command = arguments[0];
      
      if (strcmp(command, "echo") == 0){

            for(i = 1; i<MAX_COMMAND_LINE_ARGS; i++){
                if (arguments[i] == NULL){break;}
                printf("%s ", arguments[i]);
            }
        }else if (strcmp(command, "cd") == 0){

            if (arguments[1] == NULL){
                printf("Missing argument");
            }
            else{
                chdir(arguments[1]);
            }
            
        }else if (strcmp(command, "pwd") == 0){
       
            char path[4096];
           
           char* directory = getcwd(path, sizeof(path));
            printf("%s ", directory);
        
         }else if (strcmp(command, "exit") == 0){
            
            exit(0);
       
         }else if (strcmp(command, "env") == 0){
           
            for (i = 0; environ[i]!=NULL; i++){
                printf("%s \n", environ[i]);
            }
        
         }else if (strcmp(command, "setenv") == 0){
          
            char* variable_value[2];
            char* rest = arguments[1];
           
           for (i = 0; i<2; i++){
                char *chr = strsep(&rest, "=");
                variable_value[i] = chr;
            }
            setenv(variable_value[0], variable_value[1], 1);
       
         }else{
            
            pid = fork();
        
            int isBackgroundProcess = 0;
    
            int i = 0;
          
           for (i = 0; i<MAX_COMMAND_LINE_ARGS; i++){
            
             if (arguments[i] == NULL){break;}
          }
           
           
          i--;
          
            if (arguments[i]){
                if (strcmp(arguments[i], "&") == 0){
                    isBackgroundProcess = 1;
                    arguments[i] = NULL;
              
                }else{
                    isBackgroundProcess = 0;
                }
            }
//             
            int status;
           
           
           if (pid <0){
              
             printf("ERROR: Something Went Wrong: Fork");
                exit(1);
            
           }else if (pid == 0){
               
             if (execvp(arguments[0], arguments) == -1){
                    printf("execvp() FAILED: No such file or directory");
                }
      
    
       signal(SIGINT, shell_handler);	
      exit(1);
    }
      
        // 3. Create a child process which will execute the command line input

  
        // 4. The parent process should wait for the child to complete unless its a background process
       
      else{
                signal(SIGINT, shell_handler);
                
         signal(SIGALRM,shell_timer);

                alarm(10);
                
                if (isBackgroundProcess == 1){
                    waitpid(pid, &status, 0);
                }else {
                    wait(NULL);
                }
            }
        }
        printf("\n");
    }}

  /*
      
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
            // save what's being printed in a variable and then 
            // introduce a new counter variable j 
            // which continues to increment where i left off
            // and check for another $ and get int of that
            
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
        // check for $ maybe
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

void sighandler(int signum){
  printf("Caught signal %d, coming out..\n", signum);
  kill(pid, SIGKILL);
}
*/