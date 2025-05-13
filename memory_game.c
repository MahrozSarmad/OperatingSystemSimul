// memory_puzzle_game.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void play_game() {
    char board[4][4], hidden[4][4], symbols[] = "AABBCCDDEEFFGGHH";
    int used[16] = {0}, pairs = 8;

    srand((unsigned)time(NULL));
    for (int i = 0; i < 16; i++) {
        int pos;
        do { pos = rand() % 16; } while (used[pos]);
        used[pos] = 1;
        board[pos/4][pos%4] = symbols[i];
    }
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            hidden[r][c] = '*';

    while (pairs > 0) {
        printf("\nMemory Puzzle:\n");
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++)
                printf("%c ", hidden[r][c]);
            printf("\n");
        }

        int r1, c1, r2, c2;
        printf("Select first card (row col): ");
        if (scanf("%d %d", &r1, &c1) != 2) break;
        printf("Select second card (row col): ");
        if (scanf("%d %d", &r2, &c2) != 2) break;

        r1--; c1--; r2--; c2--;
        if (r1>=0 && r1<4 && c1>=0 && c1<4 &&
            r2>=0 && r2<4 && c2>=0 && c2<4 &&
            (r1!=r2 || c1!=c2) &&
            board[r1][c1] == board[r2][c2]) {
            hidden[r1][c1] = board[r1][c1];
            hidden[r2][c2] = board[r2][c2];
            pairs--;
            printf("Match!\n");
        } else {
            printf("No match.\n");
        }
    }

    if (pairs == 0)
        printf("Congratulations! You matched all pairs.\n");
}

int main() {
    int choice;
    while (1) {
        play_game();
        
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
