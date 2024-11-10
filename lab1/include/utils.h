#pragma once
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <string>

enum PipeIO {
    READ,
    WRITE
};

pid_t CreateChild();
void CreatePipe(int pipeFd[2]);
void Exec(const char *pathToChild);