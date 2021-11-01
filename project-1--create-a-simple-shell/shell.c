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

void signalHandler(int p){};
void killChild(int sigNum){
	printf("Program execution exceded 10 seconds\n\n");
	kill(getpid(), SIGKILL);
};



int main() {
    // Stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];
    char cmd_bak[MAX_COMMAND_LINE_LEN];
  
    // Stores the tokenized command line input.
    char *arguments[MAX_COMMAND_LINE_ARGS];
    int i;
	  pid_t pid;
	  signal(SIGINT, signalHandler);
	  signal(SIGALRM,killChild);
    	
    while (true) {
      
        do{
          // 0. Modify the prompt to print the current working directory
          // Print the shell prompt.
          if (getcwd(prompt,sizeof(prompt)) == NULL){
            perror("getcwd() error");
            
          }
          else{
            printf("%s>", prompt);
            fflush(stdout);
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
      
      //printf("CLI - %s \n", command_line);
      
      // 1. Tokenize the command line input (split it on whitespace)
      
      arguments[0] = strtok(command_line,delimiters);
      
      i=0;
      while(arguments[i] != NULL){
        //printf("- %s \n",arguments[i]);
        i++;
        arguments[i] = strtok(NULL,delimiters);
      }

        // 2. Implement Built-In Commands
      
      if (strcmp(arguments[0],"cd") == 0){
        
        chdir(arguments[1]);
        
      }
      
      else if (strcmp(arguments[0],"pwd") == 0){
        
        printf("%s\n", prompt);
        
        
      }
      else if (strcmp(arguments[0],"echo") == 0){
        
        i=1;
        while(arguments[i] != NULL){
          
          if(*arguments[i] == '$'){
            printf("%s ",getenv(arguments[i]+1));
          }
          
          else{
            printf("%s ",arguments[i]);
          }
          
          i++;
        }
        printf("\n\n");
        
        
      }
      else if (strcmp(arguments[0],"exit") == 0){
        
        exit(0);
        
        
      }
      else if (strcmp(arguments[0],"env") == 0){
        
        printf("%s \n",getenv(arguments[1]));
        
        
      }
      else if (strcmp(arguments[0],"setenv") == 0){
        
        printf("Do setenv\n");
        
        
      }
      
      else{
				
				pid = fork();
				
				if (pid>0){
					wait(NULL);
				}
				else if (pid<0){
					printf("Error forking \n\n");
				}
				
				else{
					alarm(10);
					if(execvp(arguments[0], arguments) == -1){
						printf("Error Command not recognized \n\n");
					}
					exit(0);
					
				}
				
        
      }
      
    
        // 3. Create a child process which will execute the command line input

  
        // 4. The parent process should wait for the child to complete unless its a background process
      
      
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

