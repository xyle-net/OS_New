#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/wait.h>

#define _GNU_SOURCE
#define MAX_LINE 80

typedef struct {
    char input[MAX_LINE];
    char output[MAX_LINE];
    char padding[4096 - 2 * MAX_LINE]; // Padding for page alignment
} SharedData;

int main() {
    int fd;
    pid_t pid1, pid2;
    sem_t *sem_write, *sem_process1, *sem_process2;
    SharedData *shared;

    // Create shared memory
    fd = shm_open("/shared_memory", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // Truncate shared memory to the size of SharedData
    if (ftruncate(fd, sizeof(SharedData)) == -1) {
        perror("ftruncate");
        close(fd);
        shm_unlink("/shared_memory");
        exit(1);
    }

    // Map shared memory
    shared = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared == MAP_FAILED) {
        perror("mmap");
        close(fd);
        shm_unlink("/shared_memory");
        exit(1);
    }
    close(fd);

    // Create semaphores
    sem_write = sem_open("/sem_write", O_CREAT | O_EXCL, 0644, 0);
    sem_process1 = sem_open("/sem_process1", O_CREAT | O_EXCL, 0644, 0);
    sem_process2 = sem_open("/sem_process2", O_CREAT | O_EXCL, 0644, 0);
    if (sem_write == SEM_FAILED || sem_process1 == SEM_FAILED || sem_process2 == SEM_FAILED) {
        perror("sem_open");
        munmap(shared, sizeof(SharedData));
        shm_unlink("/shared_memory");
        exit(1);
    }

    // Fork first child
    if ((pid1 = fork()) < 0) {
        perror("fork");
        exit(1);
    } else if (pid1 == 0) { // First child

        sem_wait(sem_write);
        printf("Child1: Recive string: %s", shared->input);


        for (int i = 0; shared->input[i] != '\0'; i++) {
            shared->input[i] = toupper(shared->input[i]);
        }
        printf("Child1: Processed string: %s", shared->input);

        sem_post(sem_process1);

        // Clean up and exit
        munmap(shared, sizeof(SharedData));
        sem_close(sem_write);
        sem_close(sem_process1);
        exit(0);
    }

    // Fork second child
    if ((pid2 = fork()) < 0) {
        perror("fork");
        exit(1);
    } else if (pid2 == 0) { // Second child

        sem_wait(sem_process1);
        printf("Child2: Recive string: %s", shared->input);

        // Process input: replace spaces with underscores
        for (int i = 0; shared->input[i] != '\0'; i++) {
            if (shared->input[i] == ' ') {
                shared->input[i] = '_';
            }
        }
        strcpy(shared->output, shared->input);
        printf("Child2: Processed string: %s", shared->output);

        sem_post(sem_process2);

        // Clean up and exit
        munmap(shared, sizeof(SharedData));
        sem_close(sem_process1);
        sem_close(sem_process2);
        exit(0);
    }

    // Parent process
    // Write input string
    printf("Parent: Enter a string: ");
    fgets(shared->input, MAX_LINE, stdin);

    sem_post(sem_write);
    sem_wait(sem_process2);
    printf("Parent: Final output: %s", shared->output);

    // Clean up
    munmap(shared, sizeof(SharedData));
    sem_close(sem_write);
    sem_close(sem_process1);
    sem_close(sem_process2);
    sem_unlink("/sem_write");
    sem_unlink("/sem_process1");
    sem_unlink("/sem_process2");
    shm_unlink("/shared_memory");

    // Wait for children
    wait(NULL);
    wait(NULL);

    return 0;
}