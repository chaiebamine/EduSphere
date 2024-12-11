#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "ansi_codes.h"
#include "main_interface.h"
#include <unistd.h>
#include "utilities/utilities.h"
// Function to clear the screen and display centered text
void showWelcomeScreen() {
    // Clear the screen
    system("cls");
    SetConsoleOutputCP(CP_UTF8);
    // Change console text attributes for color
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    // ASCII Art stored as a string
    const char *asciiArt =
        "\n██╗    ██╗███████╗██╗      ██████╗ ██████╗ ███╗   ███╗███████╗           \n"
        "██║    ██║██╔════╝██║     ██╔════╝██╔═══██╗████╗ ████║██╔════╝           \n"
        "██║ █╗ ██║█████╗  ██║     ██║     ██║   ██║██╔████╔██║█████╗             \n"
        "██║███╗██║██╔══╝  ██║     ██║     ██║   ██║██║╚██╔╝██║██╔══╝             \n"
        "╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝██║ ╚═╝ ██║███████╗           \n"
        " ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝           \n"
        "                                                                         \n"
        "                    ████████╗ ██████╗                                    \n"
        "                    ╚══██╔══╝██╔═══██╗                                   \n"
        "                       ██║   ██║   ██║                                   \n"
        "                       ██║   ██║   ██║                                   \n"
        "                       ██║   ╚██████╔╝                                   \n"
        "                       ╚═╝    ╚═════╝                                    \n"
        "                                                                         \n"
        "███████╗██████╗ ██╗   ██╗███████╗██████╗ ██╗  ██╗███████╗██████╗ ███████╗\n"
        "██╔════╝██╔══██╗██║   ██║██╔════╝██╔══██╗██║  ██║██╔════╝██╔══██╗██╔════╝\n"
        "█████╗  ██║  ██║██║   ██║███████╗██████╔╝███████║█████╗  ██████╔╝█████╗  \n"
        "██╔══╝  ██║  ██║██║   ██║╚════██║██╔═══╝ ██╔══██║██╔══╝  ██╔══██╗██╔══╝  \n"
        "███████╗██████╔╝╚██████╔╝███████║██║     ██║  ██║███████╗██║  ██║███████╗\n"
        "╚══════╝╚═════╝  ╚═════╝ ╚══════╝╚═╝     ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝";
    printf(HIDE_CURSOR);
    // Print blank lines to center vertically
    int linesToCenter = 5; // Adjust this based on your console height
    for (int i = 0; i < linesToCenter; i++) {
        printf("\n");
    }

    // Print spaces to center horizontally
    int spacesToCenter = 20; // Adjust this based on your console width
    for (const char *line = asciiArt; *line; line++) {
        if (*line == '\n') {
            printf("\n");
            for (int i = 0; i < spacesToCenter; i++) {
                printf(" ");
            }
        } else {
            putchar(*line);
        }
    }
    stopMP3();
    playMP3("../sound/welcome-64347.mp3");
    Sleep(2000); // Wait for 5 seconds
    stopMP3();
    // Reset the console text color
    printf(SHOW_CURSOR);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

int main(void) {
    system("title EduSphere");
    sleep(15);
    // Show the welcome screen
    showWelcomeScreen();

    // Call the main UI
    main_ui();

    // Pause the system
    system("pause");

    return 0;
}
