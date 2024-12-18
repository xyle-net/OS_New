#include <iostream>
#include <string>
#include <unistd.h>

int main(){
    std::string line;
    std::getline(std::cin, line);
    if (line.empty() || line.size() == 1) {
        std::cout << line << '\0';
        exit(EXIT_SUCCESS);
    }
    size_t i = 0;
    while (i < line.size() - 1) {
        if (line[i - 1] == ' ' && line[i] == ' ') {
            line.erase(i, 1);
            continue;
        }
        ++i;
    }

    std::cout << line << '\0';
    exit(EXIT_SUCCESS);
}