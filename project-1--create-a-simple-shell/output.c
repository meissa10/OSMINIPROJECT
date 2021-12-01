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
        bool isBackground = false;
      
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
      bool flag=false;
      while(arguments[i] != NULL){
        //printf("- %s \n",arguments[i]);
        if( strcmp(arguments[i],">") == 0){
          flag =true;
        }
        i++;
        arguments[i] = strtok(NULL,delimiters);
        
      }
      
      
      
      

        // 2. Implement Built-In Commands
      
      if (flag){
        
        pid =fork();
        
        if (pid>0){
          wait(NULL);
        }
        else if (pid<0){
          printf("Error forking >\n");
          
        }
        
        else{
          int out, status, pipefd[2];
          pid_t first;
          char *cat_args[MAX_COMMAND_LINE_ARGS]; 

          int q=0;

          while(strcmp(arguments[q],">") != 0){
            cat_args[q]=arguments[q];
            //cat_args[q] = NULL;
            //printf("%s\n",cat_args[q]);
            q++;
          }
          cat_args[q] =NULL;
          cat_args[q+1] =NULL;
          



          out = open(arguments[q+1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
          //out = fopen(arguments[p+1], "w");

          


          if (out < 0){
              fprintf(stderr, "Error opening output file\n");
              printf("Error opening output file\n");
              exit(1);
            }

          if (pipe(pipefd) != 0){
              fprintf(stderr, "Error creating pipe.\n");
              printf( "Error creating pipe.\n");
              exit(1);
            }
          
          dup2(out, 1);
          
          //close(out);
          

          execvp(cat_args[0], cat_args);
          
          
          exit(0);
        }
   
      }
      else if (strcmp(arguments[0],"cd") == 0){
        
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
        if (arguments[1] == NULL){
            for (i = 0; environ[i] != NULL; i++){
                printf("\n %s", environ[i]);
             }
            
          } 
        else{
              printf("%s \n",getenv(arguments[1]));
            
          }
        printf("\n");
      }
      else if (strcmp(arguments[0],"setenv") == 0){
        
            const char *str_arr[2];
            char *str = arguments[1];
            i=0;
            str_arr[i] = strtok(str, "=");
            while(str_arr[i] != NULL) {
              str_arr[++i] = strtok(NULL,"=");
            }
            setenv(str_arr[0],str_arr[1],1);
        
        
      }
      
      else{
        if (strcmp(arguments[i-1],"&") == 0){
           arguments[i-1] = NULL;
           isBackground = true;
            
          }
				
				pid = fork();
				
				if (pid>0){
					if (isBackground  != true){
              wait(NULL);
            }
				}
				else if (pid<0){
					printf("Error forking \n\n");
				}
				
				else{
					alarm(10);
					if(execvp(arguments[0], arguments) == -1){
						printf("Error Command not recognized \n\n");
					}
					exit(1);
					
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

