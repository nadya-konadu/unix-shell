/*
* This file contains the C source code for the shell.
*
* Author: Tu Tran, Nadya Konadu
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include <fcntl.h>

#define MAX_TOKENS 101
#define MAX_INPUT_SIZE 1024

pid_t child_pid = -1;

/*
* This function checks if a character is an operator (<, >, |, &)
*/
bool is_operator(char c) {
    return (c == '|' || c == '>' || c == '<' || c == '&');
}

/*
* This function checks if a string is one of the shell operators ("|", ">", "<", "&")
*/
bool is_operator_string(const char *str) {
    return (strcmp(str, "|") == 0 || strcmp(str, ">") == 0 ||
            strcmp(str, "<") == 0 || strcmp(str, "&") == 0);
}

/*
* This function parses the input from the command line into tokens (splitting by spaces or operators)
*/
void parse_input(char *input_line, char *tokens[]) {
    int i = 0;
    int j = 0;
    char temp_token[MAX_INPUT_SIZE];
    bool in_token = false;


    for (int k = 0; k < strlen(input_line); k++) {
        char current_char = input_line[k];

        if (isspace(current_char)) {
            if (in_token) {
                temp_token[j] = '\0';
                tokens[i++] = strdup(temp_token);
                j = 0;
                in_token = false;
            }
            continue;
        }

        if (is_operator(current_char)) {
            if (in_token) {
                temp_token[j] = '\0';
                tokens[i++] = strdup(temp_token);
                j = 0;
                in_token = false;
            }

            temp_token[0] = current_char;
            temp_token[1] = '\0';
            tokens[i++] = strdup(temp_token);
        }

        else {
            temp_token[j++] = current_char;
            in_token = true;
        }
    }
    if (in_token) {
        temp_token[j] = '\0';
        tokens[i++] = strdup(temp_token);
    }

    tokens[i] = NULL;

}

/*
* This function prompts the user for input and read the input line
*/
bool get_input(char *input_line) {
    printf("$ ");
    if (fgets(input_line, MAX_INPUT_SIZE, stdin) == NULL) {
        return false;
    }

    input_line[strcspn(input_line, "\n")] = '\0';

    return true;
}

/*
* This function handles the exit command
*/
void exit_command(){
    printf("Exiting shell...\n");
    exit(0);
}

/*
* This function displays information about the current process
*/
void myinfo_command(){
    pid_t pid = getpid();
    pid_t ppid = getppid();
    printf("PID: %d\nPPID: %d\n", pid, ppid);
}

/*
* This function changes the current directory
*/
void cd_command(char *path){
     if (path == NULL) {
        path = getenv("HOME");
     }

     if (path == NULL) {
    fprintf(stderr, "Error: HOME environment variable not set.\n");
    return;
    }

     if (chdir(path) != 0){
        perror( "cd failed");
     }
}

/*
* This function handles SIGINT (Ctrl+C) to terminate child processes
*/
void sigint_handler(int sig) {
    if (child_pid > 0) {
        printf("\nSIGINT received. Terminating child process with PID %d...\n", child_pid);
        kill(child_pid, SIGTERM);
        child_pid = -1;
    }
}

/**
* This function handles input and output redirection in a pipeline.
*/
int handle_redirection(char *tokens[], int *input_fd, int *output_fd) {
    int i = 0;
    while (tokens[i] != NULL) {
        if (strcmp(tokens[i], "<") == 0) {
            if (tokens[i + 1] == NULL) {
                fprintf(stderr, "Error: No file specified for input redirection\n");
                return -1;
            }
            *input_fd = open(tokens[i + 1], O_RDONLY);
            if (*input_fd == -1) {
                perror("Error opening input file");
                return -1;
            }
            tokens[i] = NULL;
            break;
        }
        i++;
    }

    i = 0;
    while (tokens[i] != NULL) {
        if (strcmp(tokens[i], ">") == 0) {
            if (tokens[i + 1] == NULL) {
                fprintf(stderr, "Error: No file specified for output redirection\n");
                return -1;
            }
            *output_fd = open(tokens[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (*output_fd == -1) {
                perror("Error opening output file");
                return -1;
            }
            tokens[i] = NULL;
            break;
        }
        i++;
    }
    return 0;
}


/*
* This function executes commands, including handling redirection, background execution, and piping.
*/
int execute_command(char *tokens[]) {
    if (tokens[0] == NULL) {
        fprintf(stderr, "Error: No command entered\n");
        return -1;
    }

    if (strcmp(tokens[0], "exit") == 0) {
        exit_command();
        return 0;
    }

    if (strcmp(tokens[0], "myinfo") == 0) {
        myinfo_command();
        return 0;
    }

    if (strcmp(tokens[0], "cd") == 0) {
        char *path = tokens[1] ? tokens[1] : NULL;
        cd_command(path);
        return 0;
    }

    bool background = false;
    int last_token = 0;
    for (int i = 0; tokens[i] != NULL; i++) {
        last_token = i;
    }

    if (strcmp(tokens[last_token], "&") == 0) {
        background = true; 
        tokens[last_token] = NULL; 
    }

    int pipe_fd[2];
    pid_t pid, pid1, pid2;
    int status;

    int pipe_index = -1;
    for (int i = 0; tokens[i] != NULL; i++) {
        if (strcmp(tokens[i], "|") == 0) {
            pipe_index = i;
            break;
        }
    }

    if (pipe_index != -1) {
        for (int i = 0; i < pipe_index; i++) {
            if (strcmp(tokens[i], ">") == 0) {
                fprintf(stderr, "Error: Output redirection must be the last command in a pipeline\n");
                return -1;
        }
            }
    
         for (int i = pipe_index + 1; tokens[i] != NULL; i++) {
            if (strcmp(tokens[i], "<") == 0) {
                fprintf(stderr, "Error: Input redirection must be the first command in a pipeline\n");
                return -1; 
            }
        }
    }

    if (pipe_index != -1) {
        tokens[pipe_index] = NULL; 
        char *right_tokens[MAX_TOKENS];

        int i = 0;
        int input_fd = -1, output_fd = -1;
        for (int j = pipe_index + 1; tokens[j] != NULL; j++) {
            right_tokens[i++] = tokens[j];
        }
        right_tokens[i] = NULL; 

        if (handle_redirection(tokens, &input_fd, &output_fd) == -1) {
            return -1;
        }

        if (pipe(pipe_fd) == -1) {
            perror("pipe");
            return -1;
        }

        if ((pid1 = fork()) == -1) {
            perror("fork");
            return -1;
        }

        if (pid1 == 0) {
            close(pipe_fd[0]); 
            dup2(pipe_fd[1], STDOUT_FILENO);
            if (input_fd != -1) {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            close(pipe_fd[1]);

            if (execvp(tokens[0], tokens) == -1) {
                perror("execvp");
                exit(1);
            }
        }

        if ((pid2 = fork()) == -1) {
            perror("fork");
            return -1;
        }

        if (pid2 == 0) {
            close(pipe_fd[1]); 
            dup2(pipe_fd[0], STDIN_FILENO);
            if (output_fd != -1) {
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
        }
            close(pipe_fd[0]);

            if (execvp(right_tokens[0], right_tokens) == -1) {
                perror("execvp");
                exit(1);
            }
        }
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        waitpid(pid1, &status, 0);
        waitpid(pid2, &status, 0);


        return 0;
    } 
    else {

        int input_fd = -1, output_fd = -1;

        if (handle_redirection(tokens, &input_fd, &output_fd) == -1) {
            return -1;
    }
    
        pid = fork();
        if (pid == -1) {
            perror("fork failed");
            return -1;
        }

        if (pid == 0) {
            if (input_fd != -1) {
                if (dup2(input_fd, STDIN_FILENO) == -1) {
                    perror("dup2 input failed");
                    exit(1);
                }
                close(input_fd);
            }

            if (output_fd != -1) {
                if (dup2(output_fd, STDOUT_FILENO) == -1) {
                    perror("dup2 output failed");
                    exit(1);
                }
                close(output_fd);
            }

            if (execvp(tokens[0], tokens) == -1) {  
                perror("execvp failed");
                exit(1);
            }
        } else {
            if (!background) {
                child_pid = pid;
                waitpid(pid, NULL, 0); 
                child_pid = -1;
            } else {
                printf("Background process started, PID %d\n", pid);
            }
        }

        return 0;
    }
}

/*
* The main function that contains the while loop, reading commands from the user and carrying out those
*/
int main() {
    char input_line[MAX_INPUT_SIZE];
    char *tokens[MAX_TOKENS];

    signal(SIGINT, sigint_handler);
   

    while (true) {
        pid_t zombie_pid;
        while ((zombie_pid = waitpid(-1, NULL, WNOHANG)) > 0) {
            printf("Reaped zombie, PID %d\n", zombie_pid);
        }

        if (!get_input(input_line)) {
            printf("\nExiting shell...\n");
            break;
        }

        if (strlen(input_line) == 0) {
            continue;
        }

        parse_input(input_line, tokens);

        printf("Tokens:\n");
        for (int i = 0; tokens[i] != NULL; i++) {
            printf("tokens[%d] = \"%s\"\n", i, tokens[i]);
        }

        execute_command(tokens);
    }

    return 0;
}