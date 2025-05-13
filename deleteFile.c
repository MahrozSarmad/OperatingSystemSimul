#include <stdio.h>
#include <stdlib.h>

// Function to delete the file with the given filename
void deleteFile() {
    char filename[100];

    // Ask the user to input the name of the file to delete
    printf("Enter the name of the file to delete: ");
    scanf("%s", filename);  // Read the filename

    // Delete the file
    if (remove(filename) == 0) {
        printf("File '%s' deleted successfully.\n", filename);
    } else {
        printf("Error deleting the file '%s'. File may not exist.\n", filename);
    }
}

int main() {
    int choice;
    while (1) {
        deleteFile();
        
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
