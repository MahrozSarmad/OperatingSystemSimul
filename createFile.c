#include <stdio.h>
#include <stdlib.h>

// Function to create a file with the given filename
void createFile() {
    FILE *file;
    char filename[100];

    // Ask the user to input the name of the file
    printf("Enter the name of the file to create: ");
    scanf("%s", filename);  // Read the filename

    // Open the file in write mode ("w"). If the file doesn't exist, it will be created.
    file = fopen(filename, "w");

    // Check if the file was successfully opened
    if (file == NULL) {
        // If file can't be created/opened, print an error and exit
        printf("Error creating the file!\n");
        return;
    }

    // Close the file immediately as no content is written
    fclose(file);

    printf("File '%s' created successfully with no content.\n", filename);
}

int main() {
    int choice;
    while (1) {
        createFile();
        
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
