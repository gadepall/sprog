#include <stdio.h>
#define N 2 // Size of the matrix

void LU_Decomposition(float mat[N][N], float lower[N][N], float upper[N][N]) {
    // Initialize lower and upper matrices
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j)
                lower[i][j] = 1; // Diagonal elements of L are 1
            else
                lower[i][j] = 0;
            upper[i][j] = 0;
        }
    }

    for (int i = 0; i < N; i++) {
        // Calculate upper matrix
        for (int k = i; k < N; k++) {
            float sum = 0;
            for (int j = 0; j < i; j++) {
                sum += (lower[i][j] * upper[j][k]);
            }
            upper[i][k] = mat[i][k] - sum;
        }

        // Calculate lower matrix
        for (int k = i + 1; k < N; k++) {
            float sum = 0;
            for (int j = 0; j < i; j++) {
                sum += (lower[k][j] * upper[j][i]);
            }
            lower[k][i] = (mat[k][i] - sum) / upper[i][i];
        }
    }
}

void printMatrix(float mat[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%0.2f\t", mat[i][j]);
        }
        printf("\n");
    }
}

int main() {
    float mat[N][N];
    float lower[N][N], upper[N][N];

    printf("Enter the elements of the %dx%d matrix row by row:\n", N, N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("Element [%d][%d]: ", i + 1, j + 1);
            scanf("%f", &mat[i][j]);
        }
    }

    LU_Decomposition(mat, lower, upper);

    printf("Input Matrix:\n");
    printMatrix(mat);

    printf("\nLower Triangular Matrix:\n");
    printMatrix(lower);

    printf("\nUpper Triangular Matrix:\n");
    printMatrix(upper);

    return 0;
}

