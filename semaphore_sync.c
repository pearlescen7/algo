/****************************
* Author: Uzay Uysal        *
* Student Number: 150180039 *
****************************/

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif // !_GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>


#define KEYSEM ftok(argv[0], 1)
#define KEYSEM2 ftok(argv[0], 2)
#define KEYSHM ftok(argv[0], 3)

//Signaling function for semaphores
void sem_signal(int semid, int val)
{
    struct sembuf semaphore;
    semaphore.sem_num = 0;
    semaphore.sem_op = val;
    semaphore.sem_flg = 0;
    semop(semid, &semaphore, 1);
}

//Waiting function for semaphores
void sem_wait(int semid, int val)
{
    struct sembuf semaphore;
    semaphore.sem_num = 0;
    semaphore.sem_op = (-1 * val);
    semaphore.sem_flg = 0;
    semop(semid, &semaphore, 1);
}

int main(int argc, char *argv[])
{
    //init vars
    int shmid = 0;
    int *globalcp = NULL;
    int localInt;
    int pcSem = 0, lock = 0;
    int f;
    int child[2];
    int i, myOrder = 0;

    FILE *infile = NULL, *outfile = NULL;

    //if no input read the default file names
    if (argc < 3)
    {
        infile = fopen("./input.txt", "r");
        outfile = fopen("./output.txt", "w");
    }
    //else process the input from files
    else
    {
        char read_buff[255], write_buff[255];
        char* dotslash = "./";

        strcpy(read_buff, dotslash);
        strcpy(write_buff, dotslash);

        strcat(read_buff, argv[1]);
        strcat(write_buff, argv[2]);

        infile = fopen(read_buff, "r");
        outfile = fopen(write_buff, "w");
    }

    if (!infile)
    {
        printf("Error: File cannot be opened.");
        return 0;
    }

    char buffer[255];
    int n, M;
    fscanf(infile, "%d\n", &M);
    fscanf(infile, "%d\n", &n);

    // sync parent and children
    pcSem = semget(KEYSEM2, 1, 0700 | IPC_CREAT);
    semctl(pcSem, 0, SETVAL, 0);

    // sync children between each other 
    // using this to write to the correct location on the mem since we don't know the sizes before iterating over the array for the first time
    lock = semget(KEYSEM, 1, 0700 | IPC_CREAT);
    semctl(lock, 0, SETVAL, 0);

    // create a shared memory area
    // 2*n + 4
    // n M x y => 4 int
    // n => array A
    // n => array B + C  
    shmid = shmget(KEYSHM, ((2 * n) + 4) * sizeof(int), 0700 | IPC_CREAT);

    // attach shared mem to parent
    globalcp = (int *)shmat(shmid, 0, 0);
    *globalcp = n;
    *(globalcp + 1) = M;

    int temp, z;

    // write the input array to the shared mem => globalcp + 4 => location of A
    for (z = 0; z < n; z++)
    {
        if (z == n - 1) fscanf(infile, "%d\n", &temp);
        else fscanf(infile, "%d ", &temp);
        *(globalcp + 4 + z) = temp;
    }

    fclose(infile);

    // create 2 child processes
    for (i = 0; i < 2; ++i)
    {
        f = fork();
        if (f < 0)
        {
            printf("Error: Couldn't fork children successfully.\n");
            exit(1);
        }
        if (f == 0)
            break;
        child[i] = f;
    }

    // if parent
    if (f != 0)
    {

        // start children
        sem_signal(pcSem, 2); 

        // wait until children are done
        sem_wait(pcSem, 4);

        // write the results to file
        fprintf(outfile, "%d\n%d\n", M, n);
        for (i = 0; i < n; i++)
            fprintf(outfile, "%d ", *(globalcp + i + 4));
            
        fprintf(outfile, "\n%d\n", *(globalcp + 2));
        for (i = 0; i < *(globalcp + 2); i++)
            fprintf(outfile, "%d ", *(globalcp + i + 4 + n));

        fprintf(outfile, "\n%d\n", *(globalcp + 3));
        for (i = 0; i < *(globalcp + 3); i++)
            fprintf(outfile, "%d ", *(globalcp + i + 4 + n + *(globalcp + 2)));

        fclose(outfile);

        // detach shared mem
        shmdt(globalcp);

        // remove semaphores and shared mem
        semctl(pcSem, 0, IPC_RMID, 0);
        semctl(lock, 0, IPC_RMID, 0);
        shmctl(shmid, IPC_RMID, 0);

        // exit parent process
        exit(0);
    }
    else
    {
        // get child number
        myOrder = i;

        // get the semaphores
        lock = semget(KEYSEM, 1, 0);
        pcSem = semget(KEYSEM2, 1, 0);

        // wait for parent to get permission
        sem_wait(pcSem, 1);

        // get shared mem
        shmid = shmget(KEYSHM, ((2 * n) + 4) * sizeof(int), 0);

        // attach shared mem to use
        globalcp = (int *)shmat(shmid, 0, 0);

        // first child -> check for values smaller than M and count x
        if (myOrder == 0) 
        {
            int x = 0;
            for (i = 0; i < *globalcp; i++)
            {
                // if A[i] <= M
                if (*(globalcp + 4 + i) <= *(globalcp + 1))
                {
                    x++;
                }
            }
            *(globalcp + 2) = x;

            // other child shouldn't start to write the array before x is counted
            sem_signal(lock, 1); 

            // write the array B to the memory
            int j = 0;
            for (i = 0, j = 0; i < n && j < x; i++)
            {
                // if A[i] <= M
                if (*(globalcp + 4 + i) <= *(globalcp + 1))
                {
                    // B[j] = A[i]
                    *(globalcp + 4 + n + j) = *(globalcp + 4 + i);
                    j++;
                }
            }
        }

        // second child -> check for values bigger than M and count y
        else 
        {
            int y = 0;
            for (i = 0; i < *globalcp; i++)
            {
                // if A[i] > M
                if (*(globalcp + 4 + i) > *(globalcp + 1))
                {
                    y++;
                }
            }
            *(globalcp + 3) = y;

            // wait until x is counted
            sem_wait(lock, 1); 

            // write the array C to the memory
            int j = 0;
            for (i = 0, j = 0; i < n && j < y; i++)
            {
                // if A[i] > M
                if (*(globalcp + 4 + i) > M)
                {
                    // C[j] = A[i]
                    // + 4 to skip n M x y
                    // n to skip A
                    // x to skip B
                    *(globalcp + 4 + n + *(globalcp + 2) + j) = *(globalcp + 4 + i);
                    j++;
                }
            }
        }

        // detach shared mem
        shmdt(globalcp);

        // inc sem to signal parent
        sem_signal(pcSem, 2);

        // exit child process 
        exit(0);
    }

    return 0;
}
