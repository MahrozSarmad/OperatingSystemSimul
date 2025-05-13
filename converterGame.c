#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int num; // Global variable for storing the decimal number

// Function to ask for and validate the decimal number input
void *getDecimalInput(void *arg) {
    while (1) {
        printf("Enter a decimal number: ");
        if (scanf("%d", &num) == 1) {
            break;
        } else {
            printf("Invalid input! Please enter a valid integer.\n");
            while (getchar() != '\n');  // Clear the input buffer
        }
    }
    pthread_exit(NULL);  // Exit the thread after input is valid
}

// Function to ask the user to guess octal and hexadecimal representations
void *getConversions(void *arg) {
    char userOctal[20], userHex[20];

    // Ask user to guess the octal conversion
    printf("\nWhat is the octal representation of %d? ", num);
    scanf("%s", userOctal);

    // Ask user to guess the hexadecimal conversion
    printf("What is the hexadecimal representation of %d? ", num);
    scanf("%s", userHex);

    // Show the correct answers
    printf("\nCorrect Answers:\n");
    printf("Octal      : %o\n", num);
    printf("Hexadecimal: %X\n", num);

    pthread_exit(NULL);  // Exit the thread after giving the correct answers
}

void decimalToBinary(int decimal) {
    if (decimal > 0) {
        decimalToBinary(decimal / 2);
        printf("%d", decimal % 2);
    }
}

void decimalToHexadecimal(int decimal) {
    if (decimal > 0) {
        decimalToHexadecimal(decimal / 16);
        int remainder = decimal % 16;
        if (remainder < 10)
            printf("%d", remainder);
        else
            printf("%c", 'A' + (remainder - 10));
    }
}

int main() {
    int score = 0;
    int num_questions = 5;
    int choice;
    srand(time(NULL));

    while (1) {
        printf("\n=== Decimal Converter Game ===\n");
        printf("Convert the following decimal numbers to binary and hexadecimal!\n\n");

        for (int i = 0; i < num_questions; i++) {
            int decimal = rand() % 100 + 1;  // Random number between 1 and 100
            int user_binary, user_hex;
            char user_hex_str[10];

            printf("\nQuestion %d: Convert %d to:\n", i + 1, decimal);
            
            // Binary conversion
            printf("Binary: ");
            scanf("%d", &user_binary);
            
            // Hexadecimal conversion
            printf("Hexadecimal: ");
            scanf("%s", user_hex_str);

            // Check answers
            printf("\nCorrect answers:\n");
            printf("Binary: ");
            decimalToBinary(decimal);
            printf("\nHexadecimal: ");
            decimalToHexadecimal(decimal);
            printf("\n");

            // Add points if correct (simplified scoring)
            score += 2;
        }

        printf("\nGame Over! Your score: %d/%d\n", score, num_questions * 2);

        printf("\nActions:\n");
        printf("1) Continue   2) Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            score = 0;  // Reset score for new game
            continue;
        } else {
            break;
        }
    }

    return 0;
}

