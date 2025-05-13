#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void playSong(const char *songPath) {
    // Clear the terminal screen before playing the song
    system("clear");

    // Display the message indicating that the song is playing
    printf("Now playing: %s\n", songPath);

    // Try mpg123 first, if not available, fall back to ffplay
    char command[256];
    snprintf(command, sizeof(command),
             "which mpg123 >/dev/null 2>&1 && mpg123 \"%s\" >/dev/null 2>&1 || ffplay -nodisp -autoexit \"%s\" >/dev/null 2>&1",
             songPath, songPath);
    
    int result = system(command);
    if (result != 0) {
        printf("Error playing file. Make sure mpg123 or ffplay is installed.\n");
        printf("You can install them using:\n");
        printf("sudo apt-get install mpg123\n");
        printf("or\n");
        printf("sudo apt-get install ffmpeg\n");
    }
}

int main() {
    int choice;
    
    while (1) {
        printf("\n=== Music Player ===\n");
        printf("Select a song to play:\n");
        printf("1. tereNaal.mp3\n");
        printf("2. pakarmy.mp3\n");
        printf("Enter your choice (1 or 2): ");
        scanf("%d", &choice);

        const char *songPath;

        // Based on the user's choice, set the song path
        if (choice == 1) {
            songPath = "tereNaal.mp3";
        } else if (choice == 2) {
            songPath = "pakarmy.mp3";
        } else {
            printf("Invalid choice!\n");
            continue;
        }

        // Play the selected song
        playSong(songPath);

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

