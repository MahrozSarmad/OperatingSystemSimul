#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char oldName[256];
    char newName[256];
    int choice;

    while (1) {
        printf("\n=== File Renamer ===\n");
        printf("Enter current filename: ");
        scanf("%s", oldName);
        printf("Enter new filename: ");
        scanf("%s", newName);

        if (rename(oldName, newName) == 0) {
            printf("File renamed successfully.\n");
        } else {
            printf("Error renaming file.\n");
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
