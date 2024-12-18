#include <stdio.h>

// Прототипы функций
int PrimeCount(int A, int B);
float Square(float A, float B);

int main() {
    int command;
    while (1) {
        printf("Enter command: ");
        scanf("%d", &command);

        if (command == 0) {
            printf("Switching not supported in this program.\n");
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
    return 0;
}