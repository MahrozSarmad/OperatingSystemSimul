#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>  // Include pthread library

// Function to open and display contents of a file
void *openFile(void *filename_ptr) {
    const char *filename = (const char *)filename_ptr;  // Cast the argument back to a string

    FILE *file = fopen(filename, "r");  // Open the file in read mode
    if (file == NULL) {
        printf("File not found or unable to open the file.\n");
        return NULL;
    }

    printf("Contents of the file '%s':\n", filename);
    char ch;
    // Read and display the content of the file
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file);  // Close the file after reading
    return NULL;
}

// Function to edit a file (overwrite content)
void *editFile(void *filename_ptr) {
    const char *filename = (const char *)filename_ptr;  // Cast the argument back to a string

    FILE *file = fopen(filename, "w");  // Open the file in write mode to clear existing content
    if (file == NULL) {
        printf("File not found or unable to open the file.\n");
        return NULL;
    }

    // Prompt the user to enter new content
    printf("Enter the content (press Enter twice to save and finish):\n");
    char content[500];
    int empty_line_count = 0;
    
    while (1) {
        if (fgets(content, sizeof(content), stdin) == NULL) {
            break;
        }

        // Check for empty line (just Enter key)
        if (content[0] == '\n') {
            empty_line_count++;
            if (empty_line_count >= 2) {
                break;
            }
        } else {
            empty_line_count = 0;
        }

        fputs(content, file);
    }

    fclose(file);  // Close the file after writing
    printf("File '%s' has been saved successfully.\n", filename);
    return NULL;
}

// Function to ask user for file name and then choose to either open or edit it
void notepad() {
    char filename[100];
    int choice;

    // Prompt the user to enter the file name to open or edit
    printf("Enter the name of the file to open/edit: ");
    scanf("%s", filename);  // Read the filename

    FILE *file = fopen(filename, "r");  // Try to open the file in read mode
    if (file == NULL) {
        // If the file doesn't exist
        printf("The file '%s' does not exist.\n", filename);
        return;
    }

    fclose(file);  // Close the file after checking existence

    // Ask the user whether they want to open or edit the file
    printf("\nFile '%s' exists. Choose an option:\n", filename);
    printf("1. Open the file and see content\n");
    printf("2. Edit the file\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);

    pthread_t thread1, thread2;  // Declare two threads

    // Create a thread based on user choice
    if (choice == 1) {
        // Create thread to open the file
        pthread_create(&thread1, NULL, openFile, (void *)filename);
        pthread_join(thread1, NULL);  // Wait for thread to finish
    } else if (choice == 2) {
        // Create thread to edit the file
        pthread_create(&thread2, NULL, editFile, (void *)filename);
        pthread_join(thread2, NULL);  // Wait for thread to finish
    } else {
        printf("Invalid choice! Please choose either 1 or 2.\n");
    }
}

int main() {
    int choice;
    while (1) {
        notepad();
        
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