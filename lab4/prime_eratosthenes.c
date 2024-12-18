#include <math.h>
#include <string.h>

int PrimeCount(int A, int B) {
    if (B < 2) return 0;
    int count = 0;
    int n = B + 1;
    int *is_prime = malloc(n * sizeof(int));
    memset(is_prime, 1, n * sizeof(int));
    is_prime[0] = is_prime[1] = 0;

    for (int i = 2; i <= sqrt(B); i++) {
        if (is_prime[i]) {
            for (int j = i * i; j < n; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    for (int i = A; i <= B; i++) {
        if (is_prime[i]) count++;
    }

    free(is_prime);
    return count;
}