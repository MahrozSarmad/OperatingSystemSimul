#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encryptText(const char *input, char *output) {
    // Simple XOR encryption with key 42
    for (int i = 0; input[i] != '\0'; i++) {
        output[i] = input[i] ^ 42;
    }
}

void decryptText(const char *input, char *output) {
    // XOR decryption (same as encryption)
    for (int i = 0; input[i] != '\0'; i++) {
        output[i] = input[i] ^ 42;
    }
}

int main() {
    char input[1000];
    char output[1000];
    int choice;

    while (1) {
        printf("\n=== Text Encryptor ===\n");
        printf("Enter text to encrypt/decrypt: ");
        scanf(" %[^\n]s", input);

        printf("\nChoose operation:\n");
        printf("1. Encrypt\n");
        printf("2. Decrypt\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            encryptText(input, output);
            printf("\nEncrypted text: %s\n", output);
        } else if (choice == 2) {
            decryptText(input, output);
            printf("\nDecrypted text: %s\n", output);
        } else {
            printf("Invalid choice!\n");
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
