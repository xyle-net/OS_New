#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

void InitParent(const char *pathToChild1, const char *pathToChild2);

void ParentProcess(
    int *child1In,
    int *child2Out,
    int *pipeBetween
);

void Child1Process(
    const char *pathToChild1,
    int *child1In,
    int *child2Out,
    int *pipeBetween
);

void Child2Process(
    const char *pathToChild2,
    int *child1In,
    int *child2Out,
    int *pipeBetween
);