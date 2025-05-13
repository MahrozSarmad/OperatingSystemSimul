#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void playGuessGame() {
    int number, guess, attempts = 0;
    srand(time(0));
    number = rand() % 100 + 1;

    printf("Guess the number (1 to 100):\n");

    do {
        scanf("%d", &guess);
        attempts++;

        if (guess < number)
            printf("Too low! Try again: ");
        else if (guess > number)
            printf("Too high! Try again: ");
        else
            printf("Correct! You guessed it in %d attempts.\n", attempts);

    } while (guess != number);
}

int main() {
    playGuessGame();
    return 0;
}
