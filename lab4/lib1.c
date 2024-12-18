#include <math.h>

int PrimeCount(int A, int B) {
    int count = 0;
    for (int i = A; i <= B; i++) {
        if (i < 2) continue;
        int is_prime = 1;
        for (int j = 2; j <= sqrt(i); j++) {
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) count++;
    }
    return count;
}
float Square(float A, float B) {
    return A * B;
}