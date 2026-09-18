# Writing a Shell


## Author Information   
- **Names**: Nadya Konadu & Tu Tran

---

## Completed Portions  

### Task 1. Read user input  
- Successfully implemented a loop to prompt the user and read input until EOF (Ctrl-D).
- Used `fgets(3)` to read input efficiently.
- Tested various command-line inputs to ensure proper behavior.

### Task 2. Run a simple process  
- Forked a subprocess and executed single-word commands (e.g., `ls`).
- Handled user input with no command gracefully (i.e., ignored empty inputs).

### Task 3. Parse whitespace in commands  
- Parsed command strings into arrays of tokens using functions like `strtok(3)`, `strchr(3)`, `strsep(3)`, and `strpbrk(3)`.
- Tested commands with whitespace and special symbols (e.g., `ls -a | grep foo > foo.txt`).
  
### Task 4. Run multi-token commands  
- Executed multi-token commands from parsed arrays.


### Task 5. Built-in commands  
- Implemented built-in commands: `exit`, `myinfo`, and `cd`.
- Ensured `cd` worked with both relative and absolute paths.

### Task 6. Trap SIGINT  
- Trapped `SIGINT` (Ctrl-C) to terminate the currently running child process instead of the entire shell.
- Used `signal(2)` to handle signals effectively.

### Task 7. Run background processes  
- Supported background process execution (`&` at the end of commands).
- Reaped zombie processes using `waitpid(-1, &status, WNOHANG)`.

### Task 8. Output redirection  
- Handled input/output redirection (`<`, `>`) by using `open(2)` and `dup2(2)` for file descriptors.
- Tested redirection for commands with input/output redirection.

### Task 9. Piping  
- Supported command piping using `pipe(2)` and `dup2(2)`.
- Executed commands connected via pipes (`foo | bar`).
- Tested and verified correct behavior for both components of a pipe.

---

## Portions Not Completed  
- All parts were completed.
---

## Known Bugs/Errors  
1. **Redirection & Piping Issue**:  
   When using commands with both piping and output redirection, the shell is not creating the output file as expected. For example, given the command:  
   
   ```sh
   cat input.txt | grep test > output.txt


2. **Handling Quotes in Commands**:
  Commands such as grep and others should not have quotes around the words, as the code isn’t correctly processing quotation marks.

---

## External Sources & References  
### General C Programming & System Calls  
- **man pages**: Used for system calls like `fork(2)`, `execvp(3)`, `waitpid(2)`, `chdir(2)`, `kill(2)`, `open(2)`, `dup2(2)`, `pipe(2)`, `strtok(3)`, etc.  
- **Stack Overflow**: Searched extensively for error handling, specific implementation details, and common pitfalls in C programming and Unix system calls.  
  - [fork() example](https://stackoverflow.com/questions/985051/what-is-the-purpose-of-fork)  
  - [dup2 usage](https://stackoverflow.com/questions/1720535/practical-examples-use-dup-or-dup2)  
  - [Piping](https://www.geeksforgeeks.org/piping-in-unix-or-linux/)
  - [grep](https://www.geeksforgeeks.org/grep-command-in-unixlinux/)
  - [Making a shell in C](https://www.geeksforgeeks.org/making-linux-shell-c/)

### Piping & Redirection  
- **man dup2(2)**  
- **man pipe(2)**  
- (https://www.geeksforgeeks.org/dup-dup2-linux-system-call/)
- (https://www.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html)
- **Stack Overflow**: Solutions for piping, file descriptor management, and handling redirections (`<`, `>`, `|`).  
- [pipe and dup2 explanation](https://stackoverflow.com/questions/21033301/clarification-on-how-pipe-and-dup2-work-in-c)  
- [redirection issues](https://stackoverflow.com/questions/47478392/redirection-at-the-end-of-the-pipe-c-shell)  
- (https://www.reddit.com/r/C_Programming/comments/rz8gji/help_me_to_understand_pipes_and_redirection_but/)
- [Execute Automation - Youtube](https://www.youtube.com/watch?v=H9MLzwVJwsQ)  
- [Gary Explains - Youtube](https://www.youtube.com/watch?v=3hbqDtfI3zE)

### Tokenization & Parsing  
- **man strtok(3)**  
- **man strchr(3)**  
- **man strsep(3)**  
- **Stack Overflow**: Used to parse command lines, handle whitespace, and extract tokens.  
- [strtok usage](https://stackoverflow.com/questions/3889992/how-does-strtok-split-the-string-into-tokens-in-c)  
- **Neso Academy**: Excellent tutorials on string manipulation and tokenization in C programming.  
- [Portfolio Courses - Youtube](https://www.youtube.com/watch?v=nrO_pXGZc3Y)   

### Built-in Commands  
- **man chdir(2)**  
- **man getcwd(3)**  
- **Stack Overflow**: Used to implement built-in commands like `cd`, `exit`, and `myinfo`.  
- [cd command explanation](https://stackoverflow.com/questions/63470475/cd-vs-cd-explanation)  
- **Embedded Vibes**: Tutorials on shell commands and their behavior in Unix/Linux systems.  
- [Lazy Programer - Youtube](https://www.youtube.com/watch?v=F1KdoEoY46M)(https://www.youtube.com/watch?v=RX04ClLKunQ)  

### Background Processes & Zombie Reaping  
- **man waitpid(2)**  
- **Stack Overflow**: Used to manage background processes and reap zombie processes.  
- [waitpid with WNOHANG](https://stackoverflow.com/questions/33508997/waitpid-wnohang-wuntraced-how-do-i-use-these) (https://stackoverflow.com/questions/2353079/reaping-zombie-process-child)  
- [Jacob Sorber - Youtube](https://www.youtube.com/watch?v=_5SCtRNnf9U) (https://www.youtube.com/watch?v=xJ8KenZw2ag)


