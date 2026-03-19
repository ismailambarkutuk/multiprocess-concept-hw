// CMPE 382 - Homework Assignment 1
// İsmail Ambarkütük
// ID: 18821188760
// Section: 04

// library imports
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <time.h>
#include <sys/wait.h>


// checkes whether n (parameter)
// is prime or not.
// checks divisible from 2 up to sqrt(n)
// returns true if yes
// returns false otherwise
bool isPrime(int n) {
    if (n < 2)
        return false;

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }

    return true;
}

// checkes whether n (parameter)
// is abundant or not.
// sum all divisors of n
// returns true if yes
// returns false if not
bool isAbundant(int n) {
	int sum = 0;

    for (int i = 1; i <= n / 2; i++) {
        if (n % i == 0)
            sum += i;
    }

    return sum > n;
}

// it reads all the numbers in a file and
// counts how many of them are prime.
int countPrimesInFile(const char *filename) {

	//try to open the file in read mode (r)
    FILE *fp = fopen(filename, "r");
    int number, count = 0;

	// check if failed to open
    if (fp == NULL) {
        perror("fopen");
        return -1;
    }

	// read ints from the file one by one
    while (fscanf(fp, "%d", &number) == 1) {
        if (isPrime(number)) // prime check
            count++;
    }

    fclose(fp); // close file after reading
    return count;
}

int countAbundantsInFile(const char *filename) {

	//try to open the file in read mode (r)
    FILE *fp = fopen(filename, "r");
    int number, count = 0;

	// check if failed to open
    if (fp == NULL) {
        perror("fopen");
        return -1;
    }

	// read ints from the file one by one
    while (fscanf(fp, "%d", &number) == 1) {
        if (isAbundant(number)) // abundant check
            count++;
    }

    fclose(fp); // close file after reading
    return count;
}



void createFiles(int N, int max) {

	//open files in write mode (w)
    FILE *f1 = fopen("File1.txt", "w");
    FILE *f2 = fopen("File2.txt", "w");

	// check if failed to open
    if (f1 == NULL || f2 == NULL) {
        perror("fopen");
        exit(1);
    }

	
	// generate N random numbers for each file
    srand(1);
    for (int i = 0; i < N; i++) {
        fprintf(f1, "%d\n", rand() % (max + 1));
        fprintf(f2, "%d\n", rand() % (max + 1));
    }

	// close both files after process
    fclose(f1);
    fclose(f2);
}

int main() {
    int N;
    int max = 860; // ID=18821188760 -> last 3 digits + 100 = 860

    // Pipes:
    // p_to_c1: parent to child1
    // c1_to_p: child1 to parent
    // p_to_c2: parent to child2
    // c2_to_p: child2 to parent
    int p_to_c1[2], c1_to_p[2];
    int p_to_c2[2], c2_to_p[2];
    pid_t pid1, pid2;

    int primeCount, abundantCount;
    int receivedN;

	// get N number input from user
    printf("Enter N: ");
    if (scanf("%d", &N) != 1) {
        printf("Invalid input\n");
        return 1;
    }

	// create two files each contains N random numbers up to 860 (max)
    createFiles(N, max);

	// tries to create 4 pipes
	// if it fails gives error
    if (pipe(p_to_c1) == -1 || pipe(c1_to_p) == -1 ||
        pipe(p_to_c2) == -1 || pipe(c2_to_p) == -1) {
        perror("pipe");
        return 1;
    }

	// fork first child (P2)
    pid1 = fork();

	// error handling
    if (pid1 < 0) {
        perror("fork");
        return 1;
    }

	// CHILD PROCESS 1 (P2)
    if (pid1 == 0) {


        close(p_to_c1[1]); //close write end (parent writes)
        close(c1_to_p[0]); //close read end (parent reads)

		// close all pipes of second child
        close(p_to_c2[0]);
        close(p_to_c2[1]);
        close(c2_to_p[0]);
        close(c2_to_p[1]);

		// receive N from parent
        read(p_to_c1[0], &receivedN, sizeof(receivedN));

		// count primes in File1
        primeCount = countPrimesInFile("File1.txt");

		// send to the parent
        write(c1_to_p[1], &primeCount, sizeof(primeCount));

		// receive abundant count from parent
        read(p_to_c1[0], &abundantCount, sizeof(abundantCount));

		// compare prime and abundant count
		// print the result
        if (primeCount >= abundantCount)
            printf("I am Child process P2: The winner is child process P2\n");
        else
            printf("I am Child process P2: The winner is child process P3\n");

		// close used pipes
        close(p_to_c1[0]);
        close(c1_to_p[1]);
        exit(0);
    }


	// fork second child (P3)
    pid2 = fork();

	// error handling
    if (pid2 < 0) {
        perror("fork");
        return 1;
    }

	// CHILD PROCESS 2 (P3)
    if (pid2 == 0) {
        close(p_to_c2[1]); //close write end (parent writes)
        close(c2_to_p[0]); //close read end (parent reads)

		// close all pipes of first child
		close(p_to_c1[0]);
        close(p_to_c1[1]);
        close(c1_to_p[0]);
        close(c1_to_p[1]);

   		// receive N from parent
		read(p_to_c2[0], &receivedN, sizeof(receivedN));

		// count abundants from File2
        abundantCount = countAbundantsInFile("File2.txt");

		// send to the parent
        write(c2_to_p[1], &abundantCount, sizeof(abundantCount));

		// get prime count from parent
        read(p_to_c2[0], &primeCount, sizeof(primeCount));

		// comparison
        if (abundantCount > primeCount)
            printf("I am Child process P3: The winner is child process P3\n");
        else
            printf("I am Child process P3: The winner is child process P2\n");


 		// close used pipes
		close(p_to_c2[0]);
        close(c2_to_p[1]);
        exit(0);
    }

	// PARENT PROCESS

	// close unused pipe ends
    close(p_to_c1[0]);
    close(c1_to_p[1]);
    close(p_to_c2[0]);
    close(c2_to_p[1]);

	// send N to both children
    write(p_to_c1[1], &N, sizeof(N));
    write(p_to_c2[1], &N, sizeof(N));

	// receive results from children
    read(c1_to_p[0], &primeCount, sizeof(primeCount));
    read(c2_to_p[0], &abundantCount, sizeof(abundantCount));

	// send results back to children for comparison
    write(p_to_c1[1], &abundantCount, sizeof(abundantCount));
    write(p_to_c2[1], &primeCount, sizeof(primeCount));

	// wait for children to finish
    wait(NULL);
    wait(NULL);

	// print results
    printf("The number of positive integers in each file: %d\n", N);
    printf("The number of prime numbers in File1: %d\n", primeCount);
    printf("The number of abundant numbers in File2: %d\n", abundantCount);

	// close all remaining pipe ends
    close(p_to_c1[1]);
    close(c1_to_p[0]);
    close(p_to_c2[1]);
    close(c2_to_p[0]);

    return 0;
}
