# CMPE 382 Homework 1  
## Multiprocess Concept: `fork()` and `pipe()` System Calls

This project is a C implementation of an Operating Systems homework about **inter-process communication (IPC)** using **`fork()`** and **`pipe()`** on **GNU/Linux**.

The program creates a parent process and two child processes.  
The parent generates two files filled with random integers, sends data to the children through pipes, receives their results, and announces the final output.

---

## Project Purpose

The goal of this homework is to demonstrate:

- process creation with `fork()`
- communication between processes with `pipe()`
- parent-child coordination
- file creation and file reading in C
- counting:
  - **prime numbers** in `File1.txt`
  - **abundant numbers** in `File2.txt`

---

## How the Program Works

1. The parent process asks the user to enter `N`.
2. The parent creates:
   - `File1.txt`
   - `File2.txt`
3. Each file is filled with `N` random integers.
4. The parent creates two child processes:
   - **Child P2**
   - **Child P3**
5. The parent sends `N` to both children using pipes.
6. **Child P2** reads `File1.txt` and counts the number of prime numbers.
7. **Child P3** reads `File2.txt` and counts the number of abundant numbers.
8. Both children send their results back to the parent.
9. The parent sends each child the other child’s result.
10. Each child determines the winner and prints a message.
11. The parent waits for both children to finish and prints the final summary.

---

## Definitions

### Prime Number
A prime number is a positive integer greater than 1 that has exactly two divisors:
- 1
- itself

Examples:
- 2
- 3
- 5
- 7

---

### Abundant Number
An abundant number is a number for which the sum of its proper divisors is greater than the number itself.

Example:

12 → divisors: 1, 2, 3, 4, 6  
sum = 16  
since 16 > 12, **12 is abundant**

---

## Files

- `hw1_step5.c` → main source code
- `File1.txt` → file containing random integers for prime number counting
- `File2.txt` → file containing random integers for abundant number counting

---

## Technologies Used

- **Language:** C
- **Operating System:** GNU/Linux
- **Concepts:** Process creation, IPC, file operations

---

## Compilation

Use `gcc` to compile the program:

```bash
gcc hw1_step5.c -o step5
