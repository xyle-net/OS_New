#include <iostream>

#include "utils.h"
#include "parent.h"

int main() {
    const char PATH_TO_CHILD1[] = "C:\\Users\\mashu\\OneDrive\\Документы\\маи\\3 сем\\os\\OS\\lab1\\src\\child1.cpp";
    const char PATH_TO_CHILD2[] = "C:\\Users\\mashu\\OneDrive\\Документы\\маи\\3 сем\\os\\OS\\lab1\\src\\child2.cpp";
    InitParent(PATH_TO_CHILD1, PATH_TO_CHILD2);
    return 0;
}