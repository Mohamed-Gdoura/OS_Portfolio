# OS Portfolio - Task 1: Simple Shell

## Description
This project implements a simple Unix-like shell using C++.
It supports built-in commands, external program execution,
I/O redirection, background processes, and batch file input.

## Folder Structure

OS_Portfolio/
 └── task1/
      ├── main.cpp
      ├── help.txt
      └── myshell

## Features

- cd, dir, echo, environ, set, help, pause, quit
- External command execution using fork and exec
- Input/output redirection (<, >, >>)
- Background execution using &
- Batch file input mode

## How to Compile

```bash
cd task1
g++ main.cpp -o myshell
