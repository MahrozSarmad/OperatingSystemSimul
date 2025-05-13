// typing_speed_test.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
    const char *phrases[] = {
        "the quick brown fox jumps over the lazy dog",
        "practice makes perfect",
        "hello world",
        "typing speed test"
    };
    srand((unsigned)time(NULL));
    
    int choice;
    while (1) {
        int idx = rand() % 4;
        const char *target = phrases[idx];
        char input[256];

        printf("\nType the following phrase exactly and press enter:\n\"%s\"\n", target);
        printf("Your input: ");
        time_t start_time = time(NULL);
        if (!fgets(input, sizeof(input), stdin)) return 1;
        time_t end_time = time(NULL);

        // strip newline
        size_t len = strlen(input);
        if (len && input[len-1] == '\n') input[len-1] = '\0';

        if (strcmp(input, target) == 0) {
            double seconds = difftime(end_time, start_time);
            int chars = strlen(target);
            double cps = chars / seconds;
            double wpm = cps * 12.0 / 5.0;
            printf("Time: %.0f seconds\n", seconds);
            printf("Speed: %.1f words per minute\n", wpm);
        } else {
            printf("Input did not match. Please try again.\n");
        }

        printf("\nActions:\n");
        printf("1) Continue   2) Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar(); // Clear the newline from the input buffer

        if (choice == 1) {
            continue;
        } else {
            break;
        }
    }
    return 0;
}
