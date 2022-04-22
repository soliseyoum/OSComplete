// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str[] = "howard.edu"; 
    char input_str[100]; 

    char fixed_str2[] = "gobison.org"; 
    char input_str2[200];
    pid_t p; 
  
    if (pipe(fd1)==-1) { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    }

    printf("1st input: \n");
    scanf("%s", input_str); 
    p = fork(); 
  
    if (p < 0) { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) { 
        // fd[0] is reading end, fd[1] is writing end
            // PARENT WRITES TO FIRST PIPE

        close(fd1[0]);  // Close reading end of FIRST pipe
        close(fd2[1]); //close writing end of second pipe
  
        // Write input string to FIRST pipe. 
        write(fd1[1], input_str, strlen(input_str)+1); 
        
  
        // Wait for child to send a string 
        wait(NULL); 
  
        close(fd1[1]); // close writing end of FIRST pipe

        //read string from second pipe
        char concat_str2[100]; 
        read(fd2[0], concat_str2, 100);

        // Concatenate a fixed string with it 
        int x = strlen(concat_str2); 
        int y; 
        for (y=0; y<strlen(fixed_str2); y++) 
            concat_str2[x++] = fixed_str2[y]; 
  
        concat_str2[x] = '\0';   // string ends with '\0' 
  
        printf("2nd output:  \n%s\n", concat_str2);

        close(fd2[1]); // Close writing end of second pipe
        //close(fd1[1]); // close writing end of FIRST pipe
    } 
  

    // child process 
    else{ 
        // fd[0] is reading end, fd[1] is writing end
            // CHILD WRITES TO SECOND PIPE

        close(fd1[1]);  // Close writing end of first pipe 
        close(fd2[0]); // Close reading end of SECOND pipe
      
        // Read a string using first pipe 
        char concat_str[100]; 
        read(fd1[0], concat_str, 100); 
  
        // Concatenate a fixed string with it 
        int k = strlen(concat_str); 
        int i; 
        for (i=0; i<strlen(fixed_str); i++) 
            concat_str[k++] = fixed_str[i]; 
  
        concat_str[k] = '\0';   // string ends with '\0' 
  
        printf("1st output:  \n%s\n", concat_str);

        //get second input
        printf("2nd input: \n");
        scanf("%s", input_str2); 

        //write second input to second pipe to parent
        write(fd2[1], input_str2, strlen(input_str2)+1);

        // Wait for parent to send a string 
        wait(NULL); 

        // Close both reading ends 
        close(fd1[0]);  //close reading end of first pipe
        close(fd2[0]);  //close reading end of SECOND pipe
        close(fd2[1]); //close writnig end of SECOND pipe
  
        exit(0); 
    } 


} 
