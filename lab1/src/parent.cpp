#include "parent.h"

#include "utils.h"

#define BUFFER_SIZE (256U)

void InitParent(const char *pathToChild1, const char *pathToChild2) {
    int child1In[2], child2Out[2], pipeBetween[2];
    CreatePipe(child1In);
    CreatePipe(child2Out);
    CreatePipe(pipeBetween);

    pid_t child1Pid = fork();
    pid_t child2Pid;
    if (child1Pid == -1) {
        /* error */
        std::cerr << "Failed to create child1" << std::endl;
        exit(EXIT_FAILURE);

    } else if (child1Pid > 0) {
        /* child1 */
        Child1Process(pathToChild1, child1In, child2Out, pipeBetween);
    } else {
        /* parent */
        child2Pid = fork();
        if (child2Pid == -1) {
            /* error */
            std::cerr << "Failed to create child2" << std::endl;
            exit(EXIT_FAILURE);
        } else if (child2Pid > 0) {
            /* child2 */
            Child2Process(pathToChild2, child1In, child2Out, pipeBetween);
        } else {
            ParentProcess(child1In, child2Out, pipeBetween);
        }
    }
}

void ParentProcess(int *child1In, int *child2Out, int *pipeBetween) {
    close(child1In[READ]);
    // close(child1In[WRITE]);
    // close(child2Out[READ]);
    close(child2Out[WRITE]);
    close(pipeBetween[READ]);
    close(pipeBetween[WRITE]);

    std::string line;
    std::getline(std::cin, line);
    write(child1In[WRITE], line.c_str(), line.size());
    close(child1In[WRITE]);

    char buffer[BUFFER_SIZE];
    size_t buffer_size = BUFFER_SIZE;
    ssize_t read_result = read(child2Out[READ], buffer, buffer_size);
    if (read_result == -1) {
        std::cerr << "Failed to read" << std::endl;
        exit(EXIT_FAILURE);
    } else {
        std::cout << buffer << std::endl;
    }
    close(child2Out[READ]);
}

void Child1Process(const char *pathToChild1, int *child1In, int *child2Out,
                   int *pipeBetween) {
    // close(child1In[READ]);
    close(child1In[WRITE]);
    close(child2Out[READ]);
    close(child2Out[WRITE]);
    close(pipeBetween[READ]);
    // close(pipeBetween[WRITE]);
    int result;

    result = dup2(child1In[READ], STDIN_FILENO);
    if (result == -1) {
        std::cerr << "Child1: Failed to dublicate child1In" << std::endl;
        exit(EXIT_FAILURE);
    }
    close(child1In[READ]);

    result = dup2(pipeBetween[WRITE], STDOUT_FILENO);
    if (result == -1) {
        std::cerr << "Child1: Failed to dublicate child2Out" << std::endl;
        exit(EXIT_FAILURE);
    } 
    close(pipeBetween[WRITE]);
    Exec(pathToChild1);
}

void Child2Process(const char *pathToChild2, int *child1In, int *child2Out,
                   int *pipeBetween) {
    close(child1In[READ]);
    close(child1In[WRITE]);
    close(child2Out[READ]);
    // close(child2Out[WRITE]);
    // close(pipeBetween[READ]);
    close(pipeBetween[WRITE]);
    int result;

    result = dup2(pipeBetween[READ], STDIN_FILENO);
    if (result == -1) {
        std::cerr << "Child2: Failed to dublicate pipeBetween" << std::endl;
        exit(EXIT_FAILURE);
    }
    close(pipeBetween[READ]);

    result = dup2(child2Out[WRITE], STDOUT_FILENO);
    if (result == -1) {
        std::cerr << "Child2: Failed to dublicate child1In" << std::endl;
        exit(EXIT_FAILURE);
    }
    close(child2Out[WRITE]);
    Exec(pathToChild2);
}