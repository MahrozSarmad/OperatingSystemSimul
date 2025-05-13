#include <stdio.h>
#include <time.h>

void showTime() {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    printf("Current Date and Time: %s\n", asctime(timeinfo));
}

int main() {
    int choice;
    while (1) {
        showTime();
        
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