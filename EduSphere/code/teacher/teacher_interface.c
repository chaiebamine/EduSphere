#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>
#include <conio.h>
#include <time.h>
#include "../ansi_codes.h"
#include "../utilities/utilities.h"
#include "../notifications/notification.h"
#include "../authentification/auth.h"
#include "teacher_interface.h"

#include "../Eliminations/eliminations.h"


void viewTPersonalDetails(teacher teacher_logged_in,teachers* teachers_login) {
    system("cls");
    printTitle("Personal Details");
    printf("CIN: %s\n", teacher_logged_in.cin);
    printf("First Name: %s\n", teacher_logged_in.first_name);
    printf("Last Name: %s\n", teacher_logged_in.last_name);
    printf("Date of Birth: %04d-%02d-%02d\n",
           teacher_logged_in.date_of_birth.year,
           teacher_logged_in.date_of_birth.month,
           teacher_logged_in.date_of_birth.day);
    printf("Gender: %c\n", teacher_logged_in.gender);
    printf("Email: %s\n", teacher_logged_in.email);
    printf("Phone Number: %s\n", teacher_logged_in.phone_number);
    printf("Address: %s\n", teacher_logged_in.address);
    printf("City: %s\n", teacher_logged_in.city);
    printf("Subject: %s\n", teacher_logged_in.subject);
    printf("Username: %s\n", teacher_logged_in.username);
    printDivider();
}

// Function to edit allowed personal details
void editTPersonalDetails(teacher* teacher_logged_in,teachers** teachers_login,students* students) {
    system("cls");
    int choice;
    char enteredPassword[100];

    printf(BRIGHT_YELLOW"Enter your current password for verification: "RESET);
    getPassword(enteredPassword, sizeof(enteredPassword));

    // Verify password
    if (strcmp(teacher_logged_in->password, enteredPassword) != 0) {
        stopMP3();
        playMP3("../sound/access-denied-102628.mp3");
        printError("Incorrect password. Access denied.\n");
        sleep(2);
        viewTPersonalDetails(*teacher_logged_in,*teachers_login);
    }

    do {

        stopMP3();
        playMP3("../sound/access-granted-87075.mp3");
        sleep(1);
        system("cls");
        printTitle("Edit Personal Details");
        printMenuOption(1,"Edit Email",1);
        printMenuOption(2,"Edit Phone Number",1);
        printMenuOption(3,"Edit Address",1);
        printMenuOption(4,"Edit Username",1);
        printMenuOption(5,"Edit Password",1);
        printMenuOption(0,"Return to Student Menu",1);
        getChoice(&choice);
        switch (choice) {
            case 1:
                stopMP3();
                playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                sleep(1);
                do {
                    printf(BRIGHT_YELLOW"Enter new email: "RESET);
                    scanf(" %[^\n]s", teacher_logged_in->email);
                }while (!validateEmail(teacher_logged_in->email));
                    simulateProgressBar(2);
                    printSuccess("Email updated successfully.");
                    break;
            case 2:
                stopMP3();
                playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                sleep(1);
                printf(BRIGHT_YELLOW"Enter new phone number: "RESET);
                scanf(" %[^\n]s", teacher_logged_in->phone_number);
                simulateProgressBar(2);
                printSuccess("Phone number updated successfully.");
                break;
            case 3:
                stopMP3();
                playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                sleep(1);
                printf(BRIGHT_YELLOW"Enter new address: "RESET);
                scanf(" %[^\n]s", teacher_logged_in->address);
                simulateProgressBar(2);
                printSuccess("Address updated successfully.");
                break;
            case 4:
                stopMP3();
                playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                sleep(1);
                printf(BRIGHT_YELLOW"Enter new username: "RESET);
                scanf(" %[^\n]s", teacher_logged_in->username);
                simulateProgressBar(2);
                printSuccess("Username updated successfully.");
                break;
            case 5:
                stopMP3();
                playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                sleep(1);
                do {
                    printf(BRIGHT_YELLOW"Enter new password: "RESET);
                    scanf(" %[^\n]s", teacher_logged_in->password);
                }while (!validatePassword(teacher_logged_in->password));
                simulateProgressBar(2);
                printSuccess("Password updated successfully.");
                break;
            case 0:
                stopMP3();
                playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                printSuccess("Returning ...\n");
                sleep(1);
                teacher_ui(*teacher_logged_in,*teachers_login,students);
                break;
            default:
                stopMP3();
                playMP3("../sound/wrong-47985.mp3");
                printError("Invalid choice. Please try again.");
                break;
            }
        // Confirm the update
        printf(BRIGHT_YELLOW"Do you want to update another field? (y/n): "RESET);
        char cont;
        scanf(" %c", &cont);
        if (cont != 'y' && cont != 'Y') {
            saveTeachersToFile(*teachers_login, "../data/teachers.txt");
            teacher_ui(*teacher_logged_in,*teachers_login,students);
            break;
        }
    }while(choice!=0);
}

// Function to send a notification and save it to the file
void sendNotifications(students* head) {
    char date[11];
    char cin[9];
    char message[256];
    students* current = head;
    system("cls");
    printTitle("Send Notification");
    // Get the current date
    getCurrentDate(date);
    printf(BRIGHT_YELLOW"Enter the student cin: "RESET);
    scanf("%s",cin);


    while(strcmp(current->student.cin,cin)!=0 && current->next!= NULL) {
        current= current->next;
    }
    if (current->next == NULL) {
        printf("Student not found\n");
    }else {
        // Open the notifications file for appending
        FILE* file = fopen("../data/notifications.txt", "a");
        if (!file) {
            printf("Error opening notifications file for writing.\n");
            return;
        }
        // Get the notification message from the teacher
        printf("Enter the notification message: ");
        scanf(" %[^\n]s", message);  // Read the entire line including spaces
        // Write the notification to the file
        fprintf(file, "%s,%s,%s\n",cin, message,date);

        // Close the file
        fclose(file);

        printf("Notification sent successfully!\n");
    }
}

// Function to check if a notification already exists for a student
int notificationExists(const char* studentCIN, const char* message) {
    FILE* file = fopen("../data/notifications.txt", "r");
    if (!file) {
        printf("Error opening notifications file for reading.\n");
        return 0;  // Return 0 to indicate file couldn't be opened
    }

    char line[500];
    char cin[9];
    char notification[500];

    // Check each line in the file to see if the same notification exists
    while (fgets(line, sizeof(line), file)) {
        // Parse the line (assuming CSV format: CIN, notification, date)
        sscanf(line, "%8s,%499[^,],", cin, notification);

        // If the CIN and notification message match, return 1 (found)
        if (strcmp(cin, studentCIN) == 0 && strcmp(notification, message) == 0) {
            fclose(file);
            return 1;  // Notification already exists
        }
    }

    fclose(file);
    return 0;  // Notification does not exist
}

// Function to send a notification and save it to the file (without duplicates)
void sendNotification(const char* studentCIN, const char* message) {
    // Check if the notification already exists
    if (notificationExists(studentCIN, message)) {
        return;  // Do not send the notification again
    }

    char date[11];
    // Get the current date
    getCurrentDate(date);

    // Open the notifications file for appending
    FILE* file = fopen("../data/notifications.txt", "a");
    if (!file) {
        printf("Error opening notifications file for writing.\n");
        return;
    }

    // Write the notification to the file
    fprintf(file, "%s,%s,%s\n", studentCIN, message, date);

    // Close the file
    fclose(file);

}


// Function to display teacher's schedule, grouped by day
void displayTeacherSchedule(teacher teacher_logged_in) {
    TeacherScheduleNode* head = NULL;

    // Load all teacher schedules from the file
    loadTeacherSchedules("../data/TeacherSchedules.txt", &head);

    if (head == NULL) {
        printf(RED"No schedule found.\n"RESET);
        return;
    }
    system("cls");
    printTitle("Teacher's Schedule");

    // Create an array to store the classes grouped by day
    const char* days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    TeacherScheduleNode* current = head;

    // Loop through all days
    for (int i = 0; i < 5; i++) {
        int dayFound = 0;

        printf("\n%s:\n", days[i]);  // Print the day

        // Traverse the list and display the schedule for the current day
        current = head;
        while (current != NULL) {
            TeacherSchedule* schedule = &current->schedule;

            // Check if the schedule day matches the current day
            if (strcmp(schedule->day, days[i]) == 0 && strcmp(schedule->cin,teacher_logged_in.cin) == 0) {
                printf("  Subject: %s\n", schedule->subject);
                printf("  Time: %s\n", schedule->time);
                printf("  Classroom: %s\n", schedule->classroom);
                printf("  Group: %s\n", schedule->group);
                printf("---------------------------\n");
                dayFound = 1;
            }
            current = current->next;
        }

        if (!dayFound) {
            printf(RED"  No classes scheduled for this day.\n"RESET);
        }
    }
    printDivider();
}

void manageGrades(teacher teacher_logged_in,teachers* teachers,students* students) {
    char studentCIN[9];
    float Grade;
    char action;
    grade grades[MAX_GRADES];
    int gradeCount = loadGrades(grades);
    system("cls");
    printTitle("Manage Grades");
    printf("subject : %s\n",teacher_logged_in.subject);
    printf(BRIGHT_YELLOW"Enter Student CIN: "RESET);
    scanf("%8s", studentCIN);

    // Check if the subject exists for the student
    int found = 0;
    int subjectIndex = -1;
    for (int i = 0; i < gradeCount; i++) {
        if (strcmp(grades[i].CIN, studentCIN) == 0 && strcmp(grades[i].subject, teacher_logged_in.subject) == 0) {
            found = 1;
            subjectIndex = i;
            break;
        }
    }
    char ch[256];

    if (found) {
        // If subject exists, offer edit or delete
        printf("Subject already exists. Do you want to (E)dit or (D)elete the grade? (E/D): ");
        scanf(" %c", &action);

        if (action == 'E' || action == 'e') {
            // Edit grade
            printf(BRIGHT_YELLOW"Enter new grade for %s in %s: "RESET, studentCIN, teacher_logged_in.subject);
            scanf("%f", &Grade);
            if (Grade==0) {
                updateElimination(studentCIN,teacher_logged_in.subject,"Scored 0 in exam");
            }
            grades[subjectIndex].grade = Grade;  // Update the grade

            snprintf(ch,sizeof(ch),"\nGrade for %s in %s updated to %.2f.", studentCIN, teacher_logged_in.subject, Grade);
            printSuccess(ch);
        } else if (action == 'D' || action == 'd') {
            // Delete grade
            printf("Are you sure you want to delete the grade for %s in %s? (Y/N): ", studentCIN, teacher_logged_in.subject);
            char confirm;
            scanf(" %c", &confirm);

            if (confirm == 'Y' || confirm == 'y') {
                // Shift grades down to remove the deleted grade
                for (int i = subjectIndex; i < gradeCount - 1; i++) {
                    grades[i] = grades[i + 1];
                }
                gradeCount--;  // Decrease the count of grades
                snprintf(ch,sizeof(ch),"\nGrade for %s in %s deleted successfully.", studentCIN, teacher_logged_in.subject);
                printSuccess(ch);
            } else {
                printf("Grade deletion canceled.\n");
            }
        }
    } else {
        // If subject doesn't exist, only allow adding a new grade
        printf("Subject not found for student %s. Do you want to (A)dd a new grade? (Y|N): ", studentCIN);
        scanf(" %c", &action);

        if (action == 'Y' || action == 'y') {
            // Add new grade
            printf("Enter grade for %s in %s: ", studentCIN, teacher_logged_in.subject);
            scanf("%f", &Grade);
            if (Grade==0) {
                updateElimination(studentCIN,teacher_logged_in.subject,"Scored 0 in exam");
            }

            // Add the new grade to the list
            strcpy(grades[gradeCount].CIN, studentCIN);
            strcpy(grades[gradeCount].subject, teacher_logged_in.subject);
            grades[gradeCount].grade = Grade;
            gradeCount++;
            snprintf(ch,sizeof(ch),"Grade for %s in %s added successfully.\n", studentCIN, teacher_logged_in.subject);
            printSuccess(ch);
        } else if (action == 'N' || action == 'n'){
            printf("Grade addition canceled.\n");

        }
    }
    saveGradesToFile("../data/grades.txt", grades, gradeCount);
    waitForUser();
    teacher_ui(teacher_logged_in,teachers,students);
}

// Function to manage attendance
void manageAttendance(students* head, const char* subject) {
    char status;
    char date[11];
    char message[256];
    char group[15];
    snprintf(message, sizeof(message), "You have been marked absent in %s", subject);
    getCurrentDate(date);
    system("cls");
    printTitle("Manage Attendance");
    printf("Date: %s\n", date);
    printf("Subject: %s\n", subject);
    printf(BRIGHT_YELLOW"Enter the class: "RESET);
    scanf("%s",group);
    printf("Please mark attendance for each student:\n");

    // Open the file for appending attendance records
    FILE *file = fopen("../data/attendance.txt", "a");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Iterate through the linked list of students and prompt for attendance
    students* current = head;
    while (current != NULL) {
        if (strcmp(current->student.class,group)==0) {
            // Display student information
            printf("\nStudent: %s %s (CIN: %s)\n", current->student.first_name, current->student.last_name, current->student.cin);
            printf("Enter status (P for Present, A for Absent): ");
            scanf(" %c", &status);  // Use space before %c to consume any leftover newline


            // Validate input
            while (status != 'P' && status != 'A') {
                printf("Invalid input. Please enter P for Present or A for Absent: ");
                scanf(" %c", &status);
            }

            // Write the attendance record to the file
            fprintf(file, "%s,%s,%s,%s,%s,%c\n", date, current->student.cin, current->student.first_name,
                    current->student.last_name, subject, status);
            printf("Attendance for %s %s recorded as %c.\n", current->student.first_name, current->student.last_name, status);
            if (status == 'A') {
                sendNotification(current->student.cin,message);
            }
        }
            // Move to the next student in the linked list
            current = current->next;

    }
    fclose(file);
    printf("\nAttendance has been saved successfully.\n");
}

void view_T_notifications(teacher teacher_logged_in) {
    system("cls");
    printTitle("Notifications");
    displayNotifications(teacher_logged_in.cin);
    printDivider();
}

int isAssignmentValid(const char* groupName, const char* assignmentName, Assignment assignments[], int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(assignments[i].className, groupName) == 0 &&
            strcmp(assignments[i].name, assignmentName) == 0) {
            return 1;  // Assignment exists
            }
    }
    return 0;  // Assignment not found
}

void viewStudentSubmissions() {
    system("cls");
    Assignment assignments[MAX_ASSIGNMENTS];
    Submission submissions[maxSubmissions];
    int assignmentCount = loadAssignments(assignments);
    int submissionCount = loadSubmissions("../data/submitted_assignments.txt", submissions);

    if (submissionCount == 0) {
        printf("No submissions found.\n");
        return;
    }

    char groupName[10];
    char assignmentName[50];
    int found = 0;

    printTitle("View Student Submissions");

    // Input group and assignment
    printf(BRIGHT_YELLOW"Enter the group name (e.g., 1A): "RESET);
    scanf("%9s", groupName);
    printf(BRIGHT_YELLOW"Enter the assignment name: "RESET);
    scanf(" %[^\n]s", assignmentName);
    int valid = isAssignmentValid(groupName,assignmentName,assignments,assignmentCount);
    if (valid) {
        // Display submissions for the specified group and assignment
        printf("\nSubmissions for Group '%s' - Assignment '%s':\n", groupName, assignmentName);
        printf(BRIGHT_BLACK"----------------------------------------------------------------\n"RESET);
        printf("| %-10s | %-15s | %-15s | %-10s |\n", "CIN", "First Name", "Last Name", "Date");
        printf(BRIGHT_BLACK"----------------------------------------------------------------\n"RESET);

        for (int i = 0; i < submissionCount; i++) {
            if (strcmp(submissions[i].groupName, groupName) == 0 &&
                strcmp(submissions[i].assignmentName, assignmentName) == 0) {
                printf("| %-10s | %-15s | %-15s | %4d-%2d-%2d |\n",
                       submissions[i].cin,
                       submissions[i].firstName,
                       submissions[i].lastName,
                       submissions[i].submissionDate.year,submissions[i].submissionDate.month,submissions[i].submissionDate.day);
                found = 1;
                }
        }

        if (!found) {
            printf("No submissions found for this group and assignment.\n");
        }
        printDivider();
        waitForUser();
    }else {
        printError("Assignment doesn't exist.\n");
        printDivider();
        waitForUser();
    }


}

// Function to add an assignment
void addAssignment() {
    Assignment newAssignment;
    char dates[11];  // Array to hold the current date in string format
    date current_date;
system("cls");
    getCurrentDate(dates);  // Get the current date as a string
    sscanf(dates, "%4d-%2d-%2d", &current_date.year, &current_date.month, &current_date.day);
    printTitle("Add New Assignment");

    // Get group name
    printf(BRIGHT_YELLOW"Enter the group name (e.g., 1A): "RESET);
    scanf("%9s", newAssignment.className);

    // Get assignment name
    printf(BRIGHT_YELLOW"Enter the assignment name: "RESET);
    scanf(" %[^\n]s", newAssignment.name);

    // Get due date
    do {
        printf(BRIGHT_YELLOW"Enter the due date (YYYY MM DD): "RESET);
        scanf("%d %d %d", &newAssignment.dueDate.year, &newAssignment.dueDate.month, &newAssignment.dueDate.day);

        if (!isValidDate(newAssignment.dueDate) || (!isDatePassed(newAssignment.dueDate, current_date))) {
            printError("Invalid date. Please try again.");
        }
    } while ((!isValidDate(newAssignment.dueDate)) || (!isDatePassed(newAssignment.dueDate, current_date)));

    // Append the new assignment to the assignments file
    FILE* file = fopen("../data/assignments.txt", "a");
    if (!file) {
        printError("Error: Unable to open assignments file for writing.");
        return;
    }

    // Save the assignment in the format: Group,Assignment,DueDate
    fprintf(file, "%s,%s,%04d-%02d-%02d\n",
            newAssignment.className,
            newAssignment.name,
            newAssignment.dueDate.year,
            newAssignment.dueDate.month,
            newAssignment.dueDate.day);
    fclose(file);

    printf(MOVE_FORWARD(20) GREEN "Assignment '%s' for group '%s' added successfully with due date %04d-%02d-%02d.\n"RESET,
           newAssignment.name,
           newAssignment.className,
           newAssignment.dueDate.year,
           newAssignment.dueDate.month,
           newAssignment.dueDate.day);
    waitForUser();
}

// Main Teacher Interface Function
void teacher_ui(teacher teacher_logged_in,teachers* teachers,students* students) {
    int choice;
    printf(CLEAR_SCREEN MOVE_CURSOR(0,0));
    do {
        system("cls");
        printTitle("Teacher Interface");
        printMenuOption(1,"View Personal Details",1);
        printMenuOption(2,"View Schedule",1);
        printMenuOption(3,"Manage Grades",1);
        printMenuOption(4,"Manage Attendance",1);
        printMenuOption(5,"View Notifications",1);
        printMenuOption(6,"View Student Submissions",1);
        printMenuOption(7,"Add Assignment",1);
        printMenuOption(8,"Send Notifications to Students",1);
        printMenuOption(0,"Exit",1);
        getChoice(&choice);

        switch (choice) {
            case 1:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                int dchoice;
            printf(CLEAR_SCREEN MOVE_CURSOR(0,0));
            viewTPersonalDetails(teacher_logged_in,teachers);


            do {
                printMenuOption(1,"Edit Details",1);
                printMenuOption(0,"return",1);
                getChoice(&dchoice);
                switch (dchoice) {
                    case 1:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                        printf(CLEAR_SCREEN MOVE_CURSOR(0,0));
                    editTPersonalDetails(&teacher_logged_in,&teachers,students);
                    break;
                    case 0:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                        teacher_ui(teacher_logged_in,teachers,students);
                    break;
                    default:
                        stopMP3();
                    playMP3("../sound/wrong-47985.mp3");
                    printError("Invalid choice. Please try again.");
                    sleep(1);
                    break;
                }
            }while(dchoice !=0);
            break;
            case 2:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                printf(CLEAR_SCREEN MOVE_CURSOR(0,0));
                displayTeacherSchedule(teacher_logged_in);
            waitForUser();
            break;
            case 3:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                manageGrades(teacher_logged_in,teachers,students);
            break;
            case 4:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                manageAttendance(students,teacher_logged_in.subject);
            break;
            case 5:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                view_T_notifications(teacher_logged_in);
            break;
            case 6:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                viewStudentSubmissions();
            break;
            case 7:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                addAssignment();
            break;
            case 8:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                sendNotifications(students);
            break;
            case 0:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                saveTeachersToFile(teachers,"../data/teachers.txt");
            printTitle("Logging Out");
            printf("Thank you for using the system. You are being logged out");

            // Simulate a delay for a better user experience
            for (int i = 0; i < 3; i++) {
                printf(". ");
                fflush(stdout); // Force the dots to display immediately
                sleep(1);       // Wait for 1 second (on Windows, use Sleep(1000))
            }

            printSuccess("\nLogged out successfully.");
            printf(CLEAR_SCREEN MOVE_CURSOR(0,0));
            auth("teacher");
            break;
            default:
                stopMP3();
            playMP3("../sound/wrong-47985.mp3");
            printError("Invalid choice. Please try again.");
            sleep(1);
            break;
        }
    } while (choice != 0);
}

