#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/wait.h>

#define MAX_LINE 80

// Структура для хранения данных в разделяемой памяти
typedef struct {
    char input[MAX_LINE];   // Входная строка
    char output[MAX_LINE];  // Выходная строка
    char padding[4096 - 2 * MAX_LINE]; // Дополнительное место для выравнивания
} SharedData;

int main() {
    int fd;
    pid_t pid1, pid2;
    sem_t *sem_write, *sem_process1, *sem_process2;
    SharedData *shared;

    // Удаление существующих семафоров, если они есть
    sem_unlink("/sem_write");
    sem_unlink("/sem_process1");
    sem_unlink("/sem_process2");

    // Создание разделяемой памяти
    fd = shm_open("/shared_memory", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // Установка размера разделяемой памяти
    if (ftruncate(fd, sizeof(SharedData)) == -1) {
        perror("ftruncate");
        close(fd);
        shm_unlink("/shared_memory");
        exit(1);
    }

    // Отображение разделяемой памяти в адресное пространство процесса
    shared = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared == MAP_FAILED) {
        perror("mmap");
        close(fd);
        shm_unlink("/shared_memory");
        exit(1);
    }
    close(fd);

    // Создание семафоров
    sem_write = sem_open("/sem_write", O_CREAT | O_EXCL, 0644, 0);
    if (sem_write == SEM_FAILED) {
        perror("sem_open /sem_write");
        munmap(shared, sizeof(SharedData));
        shm_unlink("/shared_memory");
        exit(1);
    }

    sem_process1 = sem_open("/sem_process1", O_CREAT | O_EXCL, 0644, 0);
    if (sem_process1 == SEM_FAILED) {
        perror("sem_open /sem_process1");
        sem_close(sem_write);
        sem_unlink("/sem_write");
        munmap(shared, sizeof(SharedData));
        shm_unlink("/shared_memory");
        exit(1);
    }

    sem_process2 = sem_open("/sem_process2", O_CREAT | O_EXCL, 0644, 0);
    if (sem_process2 == SEM_FAILED) {
        perror("sem_open /sem_process2");
        sem_close(sem_write);
        sem_close(sem_process1);
        sem_unlink("/sem_write");
        sem_unlink("/sem_process1");
        munmap(shared, sizeof(SharedData));
        shm_unlink("/shared_memory");
        exit(1);
    }

    // Создание первого дочернего процесса
    if ((pid1 = fork()) < 0) {
        perror("fork");
        sem_close(sem_write);
        sem_close(sem_process1);
        sem_close(sem_process2);
        sem_unlink("/sem_write");
        sem_unlink("/sem_process1");
        sem_unlink("/sem_process2");
        munmap(shared, sizeof(SharedData));
        shm_unlink("/shared_memory");
        exit(1);
    } else if (pid1 == 0) { // Первый дочерний процесс
        execl("./process1.o", "process1", (char*)NULL);
        perror("execl child1");
        exit(1);
    }

    // Создание второго дочернего процесса
    if ((pid2 = fork()) < 0) {
        perror("fork");
        sem_close(sem_write);
        sem_close(sem_process1);
        sem_close(sem_process2);
        sem_unlink("/sem_write");
        sem_unlink("/sem_process1");
        sem_unlink("/sem_process2");
        munmap(shared, sizeof(SharedData));
        shm_unlink("/shared_memory");
        exit(1);
    } else if (pid2 == 0) { // Второй дочерний процесс
        execl("./process2.o", "process2", (char*)NULL);
        perror("execl child2");
        exit(1);
    }

    // Родительский процесс
    // Ввод строки от пользователя
    printf("Parent: Enter a string: ");
    fgets(shared->input, MAX_LINE, stdin);

    // Сигнал для семафора sem_write (уведомление дочерних процессов)
    sem_post(sem_write);

    // Ожидание завершения работы второго дочернего процесса
    sem_wait(sem_process2);

    // Чтение и вывод конечного результата
    printf("Parent: Final output: %s", shared->output);

    // Очистка ресурсов
    sem_close(sem_write);
    sem_close(sem_process1);
    sem_close(sem_process2);
    sem_unlink("/sem_write");
    sem_unlink("/sem_process1");
    sem_unlink("/sem_process2");
    munmap(shared, sizeof(SharedData));
    shm_unlink("/shared_memory");

    // Ожидание завершения дочерних процессов
    wait(NULL);
    wait(NULL);

    return 0;
}