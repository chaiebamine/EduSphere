#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utilities/utilities.h"
#include "notification.h"


// Function to load notifications from a file
int loadNotifications(notifications notifications[]) {
    FILE* file = fopen("../data/notifications.txt", "r");
    if (!file) {
        printf("Error opening file.\n");
        return 0;
    }

    char line[256];
    int count = 0;

    // Read each line from the file
    while (fgets(line, sizeof(line), file) && count < MAX_NOTIFICATIONS) {
        sscanf(line, "%8[^,],%149[^,],%d-%d-%d",
               notifications[count].cin,
               notifications[count].notification,
               &notifications[count].date.year,
               &notifications[count].date.month,
               &notifications[count].date.day);
        count++;
    }

    fclose(file);
    return count;
}

// Function to display notifications for a specific CIN
void displayNotifications(const char* cin) {
    int found = 0,count;
    notifications notifications[MAX_NOTIFICATIONS];
    count = loadNotifications(notifications);
    for (int i = 0; i < count; i++) {
        if (strcmp(notifications[i].cin, cin) == 0) {
            printf("[%04d-%02d-%02d] %s\n",
                   notifications[i].date.year,
                   notifications[i].date.month,
                   notifications[i].date.day,
                   notifications[i].notification);
            found = 1;
        }
    }
    if (!found) {
        printError("No notifications found!");
    }
}
