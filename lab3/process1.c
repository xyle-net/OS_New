#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>

#define MAX_LINE 80

typedef struct {
    char input[MAX_LINE];
    char output[MAX_LINE];
    char padding[4096 - 2 * MAX_LINE];
} SharedData;

int main() {
    int fd;
    sem_t *sem_write, *sem_process1;
    SharedData *shared;

    // Open shared memory
    fd = shm_open("/shared_memory", O_RDWR, 0);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // Map shared memory
    shared = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(1);
    }
    close(fd);

    // Open semaphores
    sem_write = sem_open("/sem_write", 0);
    sem_process1 = sem_open("/sem_process1", 0);
    if (sem_write == SEM_FAILED || sem_process1 == SEM_FAILED) {
        perror("sem_open");
        munmap(shared, sizeof(SharedData));
        exit(1);
    }

    // Wait for sem_write
    sem_wait(sem_write);


    // Convert input to uppercase
    for (int i = 0; shared->input[i] != '\0'; i++) {
        shared->input[i] = toupper(shared->input[i]);
    }
    printf("Process1: Processed string: %s", shared->input);

    // Signal sem_process1
    sem_post(sem_process1);

    // Clean up
    munmap(shared, sizeof(SharedData));
    sem_close(sem_write);
    sem_close(sem_process1);

    return 0;
}