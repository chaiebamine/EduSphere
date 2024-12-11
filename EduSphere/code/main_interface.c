#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <windows.h>
#include "ansi_codes.h"
#include "utilities/utilities.h"
#include "attendence/attendence.h"
#include "admins/admin_interface.h"
#include "authentification/auth.h"
#include "main_interface.h"

void main_ui() {
    removeExpiredAssignments();
    updateAbsences();
    system("cls");
    int choice;
    printTitleBox();

    const char *Roles[] = {"1. Student", "2. Teacher", "3. Admin", "0. Exit"};
    printButtons(4, Roles);
    do {



        fflush(stdin);
        // Reading input safely using fgets
        stopMP3();
        playMP3("../sound/6cdf334c01f54376ad2cc68a8d04e82d.mp3");

        getChoice(&choice);


        switch (choice) {
            case 1:
            stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                printSuccess(MOVE_FORWARD(20)"Welcome student");
                sleep(2); // sleep for 2 seconds
            system("cls");
                printf(CLEAR_SCREEN MOVE_CURSOR(0,0));
                auth("student");
                return;  // Exit the loop after a valid choice
            case 2:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                printSuccess(MOVE_FORWARD(20)"Welcome teacher");
                sleep(2); // sleep for 2 seconds
            system("cls");
                printf(CLEAR_SCREEN MOVE_CURSOR(0,0));
                auth("teacher");
                return;  // Exit the loop after a valid choice
            case 3:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                printSuccess(MOVE_FORWARD(20)"Welcome admin");
                sleep(2); // sleep for 2 seconds
            system("cls");
                printf(CLEAR_SCREEN MOVE_CURSOR(0,0));
                auth("admin");
                return;  // Exit the loop after a valid choice
            case 0:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                printSuccess(MOVE_FORWARD(20)"Goodbye");
                sleep(2);
                exit(0);
                  // Exit the loop and terminate
            default:
                stopMP3();
                playMP3("../sound/wrong-47985.mp3");
                printError(MOVE_BACKWARD(10)"Invalid choice. Please select a valid option (1-3 or 0 to exit).");
                sleep(1);
                break;  // Continue the loop if the choice is invalid
        }

    } while (1); // Continue looping until a valid choice is made
}

