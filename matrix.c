#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 10

// Function prototypes
void inputMatrix(int matrix[][MAX_SIZE], int rows, int cols);
void multiplyMatrices(int mat1[][MAX_SIZE], int mat2[][MAX_SIZE], int result[][MAX_SIZE], int r1, int c1, int c2);
void displayMatrix(int matrix[][MAX_SIZE], int rows, int cols);

// Function to input a matrix
void inputMatrix(int matrix[][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i + 1, j + 1);
            scanf("%d", &matrix[i][j]);
        }
    }
}

// Function to multiply two matrices
void multiplyMatrices(int mat1[][MAX_SIZE], int mat2[][MAX_SIZE], int result[][MAX_SIZE], int r1, int c1, int c2) {
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < c1; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

// Function to display a matrix
void displayMatrix(int matrix[][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int mat1[MAX_SIZE][MAX_SIZE], mat2[MAX_SIZE][MAX_SIZE], result[MAX_SIZE][MAX_SIZE];
    int r1, c1, r2, c2;
    int choice;

    while (1) {
        printf("\n=== Matrix Multiplication ===\n");
        
        printf("Enter rows and columns for first matrix: ");
        scanf("%d %d", &r1, &c1);
        
        printf("Enter rows and columns for second matrix: ");
        scanf("%d %d", &r2, &c2);

        if (c1 != r2) {
            printf("Error: Matrix multiplication not possible!\n");
        } else {
            printf("\nEnter elements of first matrix:\n");
            inputMatrix(mat1, r1, c1);

            printf("\nEnter elements of second matrix:\n");
            inputMatrix(mat2, r2, c2);

            multiplyMatrices(mat1, mat2, result, r1, c1, c2);

            printf("\nResultant Matrix:\n");
            displayMatrix(result, r1, c2);
        }

        printf("\nActions:\n");
        printf("1) Continue   2) Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            continue;
        } else {
            break;
        }
    }

    return 0;
}

