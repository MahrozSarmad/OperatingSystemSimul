#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

void show_file_info(const char *filename) {
    struct stat info;
    if (stat(filename, &info) != 0) {
        perror("Error");
        return;
    }
    printf("File: %s\n", filename);
    printf("Size: %ld bytes\n", info.st_size);
    printf("Last modified: %s", ctime(&info.st_mtime));
}

int main() {
    while (1) {
        char filename[100];
        printf("\nEnter file name: ");
        scanf("%99s", filename);
        show_file_info(filename);

        printf("\nActions:\n");
        printf("1) Continue   2) Exit\n");
        printf("Choice: ");
        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            continue;
        } else {
            break;
        }
    }
    return 0;
}

