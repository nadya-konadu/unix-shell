# Unix Shell

A Unix-style command-line shell implemented in C as a collaborative systems programming project for COMP 240 at Macalester College.

The project explores core operating systems concepts including process creation, command execution, signal handling, background processes, file descriptor management, redirection, and inter-process communication.

## Features

- Executes single- and multi-token Unix commands
- Parses command-line input into arguments
- Creates and manages child processes using `fork()`, `execvp()`, and `waitpid()`
- Implements built-in commands including `cd`, `exit`, and `myinfo`
- Handles `SIGINT` (`Ctrl-C`) without terminating the shell itself
- Supports background process execution using `&`
- Reaps completed background processes to prevent zombie processes
- Supports input and output redirection using `<` and `>`
- Supports command pipelines using `pipe()` and `dup2()`

## Technologies

- C
- Unix/Linux
- Make
- Unix System Calls

## Example Commands

```bash
ls
ls -a -l
cd ..
cat input.txt > output.txt
cat input.txt | grep test
sleep 10 &
```

## How to Run

Clone the repository:

```bash
git clone <repository-url>
cd unix-shell
```

Compile the shell:

```bash
make
```

Run the shell:

```bash
./shell
```

## Known Limitations

- Commands that combine piping and output redirection may not create the expected output file.
- Quoted arguments are not fully supported by the command parser.

## Contributors

**Nadya Konadu**  
**Tu Tran**

Developed collaboratively for COMP 240 at Macalester College.


