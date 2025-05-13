#include <stdio.h>
#include <stdlib.h>

// Function to perform the calculator operations
void calculator() {
    double num1, num2;
    int choice;

    // Prompt the user for input
    printf("Enter the first number: ");
    scanf("%lf", &num1);

    printf("Enter the second number: ");
    scanf("%lf", &num2);

    // Display the menu of operations
    printf("\nSelect an operation:\n");
    printf("1. Addition (+)\n");
    printf("2. Subtraction (-)\n");
    printf("3. Multiplication (*)\n");
    printf("4. Division (/)\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    // Perform the operation based on user input
    switch (choice) {
        case 1:
            printf("%.2lf + %.2lf = %.2lf\n", num1, num2, num1 + num2);
            break;
        case 2:
            printf("%.2lf - %.2lf = %.2lf\n", num1, num2, num1 - num2);
            break;
        case 3:
            printf("%.2lf * %.2lf = %.2lf\n", num1, num2, num1 * num2);
            break;
        case 4:
            if (num2 != 0) {
                printf("%.2lf / %.2lf = %.2lf\n", num1, num2, num1 / num2);
            } else {
                printf("Error! Division by zero.\n");
            }
            break;
        default:
            printf("Invalid choice! Please select a valid operation.\n");
            break;
    }
}

int main() {
    int choice;
    while (1) {
        calculator();
        
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
