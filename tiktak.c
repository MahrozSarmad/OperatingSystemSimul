#include <stdio.h>

char board[3][3];
char currentPlayer = 'X';

void resetBoard() {
    for (int i = 0, c = '1'; i < 3; i++) {
        for (int j = 0; j < 3; j++, c++) {
            board[i][j] = c;
        }
    }
}

void printBoard() {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        printf(" %c | %c | %c \n", board[i][0], board[i][1], board[i][2]);
        if (i < 2) printf("---|---|---\n");
    }
    printf("\n");
}

int makeMove(int cell) {
    int row = (cell - 1) / 3;
    int col = (cell - 1) % 3;

    if (cell < 1 || cell > 9 || board[row][col] == 'X' || board[row][col] == 'O') {
        return 0; // Invalid move
    }

    board[row][col] = currentPlayer;
    return 1;
}

int checkWin() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) return 1;
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) return 1;
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) return 1;
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) return 1;

    return 0;
}

int isBoardFull() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != 'X' && board[i][j] != 'O') return 0;
    return 1;
}

void switchPlayer() {
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
}

void playGame() {
    int move;
    resetBoard();

    while (1) {
        printBoard();
        printf("Player %c, enter your move (1-9): ", currentPlayer);
        scanf("%d", &move);

        if (!makeMove(move)) {
            printf("Invalid move. Try again.\n");
            continue;
        }

        if (checkWin()) {
            printBoard();
            printf("Player %c wins!\n", currentPlayer);
            break;
        }

        if (isBoardFull()) {
            printBoard();
            printf("It's a draw!\n");
            break;
        }

        switchPlayer();
    }
}

int main() {
    int choice;
    while (1) {
        playGame();
        
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
