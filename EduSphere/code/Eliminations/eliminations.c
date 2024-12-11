#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utilities/utilities.h"
#include "../teacher/teacher_interface.h"
#include "eliminations.h"

int load_eleminations(eleminations records[]) {
    // Function to load detailed attendance records from a file
    FILE* file = fopen("../data/eleminated.txt", "r");
    if (!file) {
        printf("Error opening file.\n");
        return 0;
    }

    char line[256];
    int count = 0;

    // Read each line from the file
    while (fgets(line, sizeof(line), file) && count < MAX_ATTENDANCE_RECORDS) {
        sscanf(line, "%8[^,],%19[^,],%99[^\n]",
               records[count].cin,
               records[count].subject,
               &records[count].reason);
        count++;
    }
    fclose(file);
    return count;
}

void display_eliminations(student student_logged_in) {
    int count;
    eleminations records[MAX_ATTENDANCE_RECORDS];
    count = load_eleminations(records);
    int found = 0;
    printf("eliminations for: %s\n", student_logged_in.first_name);
    for (int i = 0; i < count; i++) {
        if (strcmp(records[i].cin, student_logged_in.cin) == 0) {
            printf("Subject: %s, Reason: %s\n",
                records[i].subject, records[i].reason);
            found = 1;
        }
    }
    if (!found) {
        printf("You are not eliminated from any subject");
    }
}


// Function to update eliminations.txt with CIN, subject, and reason
void updateElimination(const char *cin, const char *subject, const char *reason) {
    FILE *file = fopen("../data/eleminated.txt", "r+");
    if (!file) {
        return;
    }

    eleminations records[1000];
    memset(records, 0, sizeof(records));
    char header[512];
    int count = 0;
    int found = 0;

    // Read the header from the file
    if (fgets(header, sizeof(header), file) == NULL) {
        fclose(file);
        return; // If no header, exit the function
    }

    // Read existing records from the file (skip header)
    while (fscanf(file, "%19[^,],%49[^,],%99[^\n]\n", records[count].cin, records[count].subject, records[count].reason) == 3) {
        count++;
        if (strcmp(records[count - 1].cin, cin) == 0 && strcmp(records[count - 1].subject, subject) == 0) {
            // If the record already exists, update the reason
            strncpy(records[count - 1].reason, reason, 100);
            found = 1;
        }
    }

    // If the record was not found, add a new one
    if (!found) {
        if (count >= 1000) {
            fclose(file);
            return;
        }
        strncpy(records[count].cin, cin, MAX_CIN_LENGTH);
        strncpy(records[count].subject, subject, MAX_SUBJECT_LENGTH);
        strncpy(records[count].reason, reason, 100);
        count++;
    }

    // Close the file and reopen it for writing (truncate it to clear previous data)
    fclose(file);

    // Open the file for writing (overwrite the file with updated records)
    file = fopen("../data/eleminated.txt", "w");
    if (!file) {
        return;
    }

    // Write the header back to the file
    fprintf(file, "%s", header);

    // Write all records back to the file
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%s\n", records[i].cin, records[i].subject, records[i].reason);

        // Send a notification for each updated record
        char message[256];
        snprintf(message, sizeof(message), "You have been eliminated from %s because %s", records[i].subject, records[i].reason);
        sendNotification(records[i].cin, message);
    }

    // Close the file
    fclose(file);
}

