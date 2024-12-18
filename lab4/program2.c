#include <stdio.h>
#include <dlfcn.h>

typedef int (*PrimeCountFunc)(int, int);
typedef float (*SquareFunc)(float, float);

int main() {
    void *prime_lib = dlopen("./libprime_naive.so", RTLD_LAZY);
    void *square_lib = dlopen("./libsquare_rectangle.so", RTLD_LAZY);

    if (!prime_lib || !square_lib) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return 1;
    }

    PrimeCountFunc PrimeCount = (PrimeCountFunc)dlsym(prime_lib, "PrimeCount");
    SquareFunc Square = (SquareFunc)dlsym(square_lib, "Square");

    if (!PrimeCount || !Square) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return 1;
    }

    int command;
    while (1) {
        printf("Enter command: ");
        scanf("%d", &command);

        if (command == 0) {
            dlclose(prime_lib);
            dlclose(square_lib);
            prime_lib = dlopen("./libprime_eratosthenes.so", RTLD_LAZY);
            square_lib = dlopen("./libsquare_triangle.so", RTLD_LAZY);
            PrimeCount = (PrimeCountFunc)dlsym(prime_lib, "PrimeCount");
            Square = (SquareFunc)dlsym(square_lib, "Square");
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

    dlclose(prime_lib);
    dlclose(square_lib);
    return 0;
}