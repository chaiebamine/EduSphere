#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ansi_codes.h"
#include "../utilities/utilities.h"
#include "../Eliminations/eliminations.h"
#include "attendence.h"

int load_attendance(attendance records[]) {
    // Function to load detailed attendance records from a file
        FILE* file = fopen("../data/attendance.txt", "r");
        if (!file) {
            printf("Error opening file.\n");
            return 0;
        }

        char line[256];
        int count = 0;

        // Read each line from the file
        while (fgets(line, sizeof(line), file) && count < MAX_ATTENDANCE_RECORDS) {
            sscanf(line, "%d-%d-%d, %8[^,], %29[^,], %29[^,], %19[^,], %c",
                   &records[count].date.year,&records[count].date.month,&records[count].date.day,
                   records[count].cin,
                   records[count].firstName,
                   records[count].lastName,
                   records[count].subject,
                   &records[count].status);
            count++;
        }

        fclose(file);
        return count;
}

int load_absences(absences records[]) {
    // Function to load detailed attendance records from a file
    FILE* file = fopen("../data/absences.txt", "r");
    if (!file) {
        printf("Error opening file.\n");
        return 0;
    }

    char line[256];
    int count = 0;

    // Read each line from the file
    while (fgets(line, sizeof(line), file) && count < MAX_ATTENDANCE_RECORDS) {
        sscanf(line, "%8[^,],%19[^,],%d",
               records[count].cin,
               records[count].subject,
               &records[count].absenceCount);
        count++;
    }
    fclose(file);
    return count;
}

void display_attendance(student student_logged_in) {
    int count;
    attendance records[MAX_ATTENDANCE_RECORDS];
    count = load_attendance(records);
    int found = 0;
    printTitle("Attendance");
    printf("attendance for: %s\n", student_logged_in.first_name);
    for (int i = 0; i < count; i++) {
        if (strcmp(records[i].cin, student_logged_in.cin) == 0) {
            printf("Date: %d-%d-%d, Subject: %s, Status: %c\n",
                records[i].date.year,records[i].date.month,records[i].date.day,
                records[i].subject, records[i].status);
            found = 1;
        }
    }
    if (!found) {
        printf(MOVE_FORWARD(35) BOLD RED"No records found for: %s %s\n"RESET, student_logged_in.first_name,student_logged_in.last_name);
    }
    printDivider();
}

void display_absences(student student_logged_in) {
    int count;
    absences records[MAX_ATTENDANCE_RECORDS];
    count = load_absences(records);
    int found = 0;
    printf("absences for: %s\n", student_logged_in.first_name);
    for (int i = 0; i < count; i++) {
        if (strcmp(records[i].cin, student_logged_in.cin) == 0) {
            printf("Subject: %s, Absence Count: %d\n",
                records[i].subject, records[i].absenceCount);
            found = 1;
        }
    }
    if (!found) {
        printf(MOVE_FORWARD(35) BOLD RED"No records found for: %s %s\n"RESET, student_logged_in.first_name,student_logged_in.last_name);
    }
    printDivider();
}

// Function to update absences and handle eliminations for students exceeding 3 absences
void updateAbsences() {
    attendance attendanceRecords[MAX_ATTENDANCE_RECORDS];
    int attendanceCount = load_attendance(attendanceRecords);
    absences absencesRecords[MAX_ATTENDANCE_RECORDS];
    int absencesCount = load_absences(absencesRecords);

    // Check if attendanceRecords array is populated
    if (attendanceCount <= 0) {
        return;
    }

    // Clear previous absence data
    absencesCount = 0;  // Reset absences count before updating

    for (int i = 0; i < attendanceCount; i++) {
        // Check for absence ('A' status)
        if (attendanceRecords[i].status == 'A') {
            int found = 0;

            // Look for an existing record for this student and subject
            for (int j = 0; j < absencesCount; j++) {
                if (strcmp(absencesRecords[j].cin, attendanceRecords[i].cin) == 0 &&
                    strcmp(absencesRecords[j].subject, attendanceRecords[i].subject) == 0) {
                    absencesRecords[j].absenceCount++;  // Increment absence count
                    found = 1;
                    break;
                }
            }

            // If no record was found, create a new one
            if (!found) {
                if (absencesCount >= MAX_ATTENDANCE_RECORDS) {
                    return;
                }
                // Add new absence record
                strncpy(absencesRecords[absencesCount].cin, attendanceRecords[i].cin, MAX_CIN_LENGTH);
                strncpy(absencesRecords[absencesCount].subject, attendanceRecords[i].subject, MAX_SUBJECT_LENGTH);
                absencesRecords[absencesCount].absenceCount = 1;
                absencesCount++;  // Increment absence count
            }
        }
    }

    // Check if any student has exceeded 3 absences and call updateElimination
    for (int i = 0; i < absencesCount; i++) {
        if (absencesRecords[i].absenceCount > 3) {
            // Call updateElimination to mark student for elimination
            updateElimination(absencesRecords[i].cin, absencesRecords[i].subject, "Exceeded 3 absences");
        }
    }

    // Open the file for writing
    FILE* file = fopen("../data/absences.txt", "w");
    if (!file) {
        return;
    }
    fprintf(file, "cin,subject,AbsenceCount\n");

    // Write the updated absence records to the file
    for (int i = 0; i < absencesCount; i++) {
        fprintf(file, "%s,%s,%d\n", absencesRecords[i].cin, absencesRecords[i].subject, absencesRecords[i].absenceCount);
    }

    fclose(file);
}
