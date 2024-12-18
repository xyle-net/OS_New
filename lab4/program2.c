#include <stdio.h>
#include <dlfcn.h>

typedef int (*PrimeCountFunc)(int, int);
typedef float (*SquareFunc)(float, float);

int main() {
    void *current_lib = dlopen("./lib1.so", RTLD_LAZY);
    if (!current_lib) {
        fprintf(stderr, "Error loading library: %s\n", dlerror());
        return 1;
    }

    PrimeCountFunc PrimeCount = (PrimeCountFunc)dlsym(current_lib, "PrimeCount");
    SquareFunc Square = (SquareFunc)dlsym(current_lib, "Square");
    if (!PrimeCount || !Square) {
        fprintf(stderr, "Error loading functions: %s\n", dlerror());
        dlclose(current_lib);
        return 1;
    }

    int command;
    while (1) {
        printf("Enter command: ");
        scanf("%d", &command);

        if (command == 0) {
            dlclose(current_lib); 
            if (current_lib == dlopen("./lib1.so", RTLD_LAZY)) {
                current_lib = dlopen("./lib2.so", RTLD_LAZY);
            } else {
                current_lib = dlopen("./lib1.so", RTLD_LAZY);
            }

            if (!current_lib) {
                fprintf(stderr, "Error loading library: %s\n", dlerror());
                return 1;
            }

            // Перезагружаем функции из новой библиотеки
            PrimeCount = (PrimeCountFunc)dlsym(current_lib, "PrimeCount");
            Square = (SquareFunc)dlsym(current_lib, "Square");
            if (!PrimeCount || !Square) {
                fprintf(stderr, "Error loading functions: %s\n", dlerror());
                dlclose(current_lib);
                return 1;
            }

            printf("Switched to another library.\n");
        } else if (command == 1) {
            int A, B;
            scanf("%d %d", &A, &B);
            printf("PrimeCount: %d\n", PrimeCount(A, B));
        } else if (command == 2) {
            float A, B;
            scanf("%f %f", &A, &B);
            printf("Square: %f\n", Square(A, B));
        } else {
            printf("Unknown command.\n");
        }
    }

    dlclose(current_lib); 
    return 0;
}