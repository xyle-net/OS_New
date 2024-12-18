#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    sem_t *sem_process1, *sem_process2;
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
    sem_process1 = sem_open("/sem_process1", 0);
    sem_process2 = sem_open("/sem_process2", 0);
    if (sem_process1 == SEM_FAILED || sem_process2 == SEM_FAILED) {
        perror("sem_open");
        munmap(shared, sizeof(SharedData));
        exit(1);
    }

    // Wait for sem_process1
    sem_wait(sem_process1);

    // Replace spaces with underscores
    for (int i = 0; shared->input[i] != '\0'; i++) {
        if (shared->input[i] == ' ') {
            shared->input[i] = '_';
        }
    }
    strcpy(shared->output, shared->input);
    printf("Process2: Processed string: %s", shared->output);

    // Signal sem_process2
    sem_post(sem_process2);

    // Clean up
    munmap(shared, sizeof(SharedData));
    sem_close(sem_process1);
    sem_close(sem_process2);

    return 0;
}