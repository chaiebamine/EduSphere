#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "../ansi_codes.h"
#include "../utilities/utilities.h"
#include "../authentification/auth.h"
#include "../attendence/attendence.h"
#include "../Eliminations/eliminations.h"
#include "../Notifications/notification.h"
#include "student_interface.h"
#include <unistd.h>

#include "../admins/admin_interface.h"
void viewPersonalDetails(student student_logged_in, students* students_login);
// Function to edit allowed personal details
void editPersonalDetails(student* student_logged_in, students** students_login) {
    system("cls");
    int choice;
    char enteredPassword[100];
    students* temp = *students_login;

    printf(BRIGHT_YELLOW"Enter your current password for verification: "RESET);
    getPassword(enteredPassword, sizeof(enteredPassword));

    // Verify password
    if (strcmp(student_logged_in->password, enteredPassword) != 0) {
        stopMP3();
        playMP3("../sound/access-denied-102628.mp3");
        printError("Incorrect password. Access denied.\n");
        sleep(1);
        viewPersonalDetails(*student_logged_in,*students_login);
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
        printMenuOption(0,"Return",1);

        fflush(stdin);
        getChoice(&choice);

        switch (choice) {
            case 1:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
            do {
                printf(BRIGHT_YELLOW"Enter new email: "RESET);
                scanf(" %[^\n]s", student_logged_in->email);
            }while (!validateEmail(student_logged_in->email));
                simulateProgressBar(2);
                printSuccess("Email updated successfully.");
                break;
            case 2:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                printf(BRIGHT_YELLOW"Enter new phone number: "RESET);
                scanf(" %[^\n]s", student_logged_in->phone_number);
            simulateProgressBar(2);
                printSuccess("Phone number updated successfully.");
                break;
            case 3:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                printf(BRIGHT_YELLOW"Enter new address: "RESET);
                scanf(" %[^\n]s", student_logged_in->address);
            simulateProgressBar(2);
                printSuccess("Address updated successfully.");
                break;
            case 4:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                printf(BRIGHT_YELLOW"Enter new username: "RESET);
                scanf(" %[^\n]s", student_logged_in->username);
            simulateProgressBar(2);
                printSuccess("Username updated successfully.");
                break;
            case 5:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                do{
                    printf(BRIGHT_YELLOW"Enter new password: "RESET);
                    scanf(" %[^\n]s", student_logged_in->password);
                }while (!validatePassword(student_logged_in->password));
                simulateProgressBar(2);
                printSuccess("Password updated successfully.");
                break;
            case 0:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                printSuccess("Returning ...\n");
            sleep(1);
                viewPersonalDetails(*student_logged_in,*students_login);
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
            saveStudentsToFile(students_login, "../data/students.txt");
            student_ui(*student_logged_in,*students_login);
            break;
        }
    } while (choice != 0);
}

// Function to view personal details
void viewPersonalDetails(student student_logged_in, students* students_login) {
    system("cls");
    printTitle("Personal Details");
    printf("CIN: %s\n", student_logged_in.cin);
    printf("First Name: %s\n", student_logged_in.first_name);
    printf("Last Name: %s\n", student_logged_in.last_name);
    printf("Date of Birth: %04d-%02d-%02d\n",
           student_logged_in.date_of_birth.year,
           student_logged_in.date_of_birth.month,
           student_logged_in.date_of_birth.day);
    printf("Gender: %c\n", student_logged_in.gender);
    printf("Email: %s\n", student_logged_in.email);
    printf("Phone Number: %s\n", student_logged_in.phone_number);
    printf("Address: %s\n", student_logged_in.address);
    printf("City: %s\n", student_logged_in.city);
    printf("Class: %s\n", student_logged_in.class);
    printf("Username: %s\n", student_logged_in.username);
    printDivider();

    int dchoice;
    do {
        printMenuOption(1,"Edit Details",1);
        printMenuOption(0,"Return to Student Menu",1);
        fflush(stdin);
        getChoice(&dchoice);

        switch (dchoice) {
            case 1:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                editPersonalDetails(&student_logged_in, &students_login);
                break;
            case 0:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                student_ui(student_logged_in,students_login);
                break;
            default:
                stopMP3();
            playMP3("../sound/wrong-47985.mp3");
                printError("Invalid choice. Please try again.");
            sleep(1);
            break;
        }
    } while (dchoice != 0);
}

// Function to display absences
void view_absences(student student_logged_in) {
    int achoice;
    do {
        system("cls");
        printTitle("Absences");
        display_absences(student_logged_in);
        printMenuOption(1,"View Detailed Attendance",1);
        printMenuOption(0,"Return to Student Menu",1);
        getChoice(&achoice);

        switch (achoice) {
            case 1:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
            system("cls");
                display_attendance(student_logged_in);
            waitForUser();
                break;
            case 0:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                return;
            default:
                stopMP3();
            playMP3("../sound/wrong-47985.mp3");
                printError("Invalid choice. Please try again.");
            sleep(1);
            break;
        }
    } while (achoice != 0);
}

// Function to logout
void logout() {
    printTitle("Logging Out");
    printf("Thank you for using the system. You are being logged out");

    for (int i = 0; i < 3; i++) {
        printf(". ");
        fflush(stdout); // Force the dots to display immediately
        sleep(1);       // Wait for 1 second
    }

    printSuccess("\nLogged out successfully.");
    printf(CLEAR_SCREEN MOVE_CURSOR(0, 0));
    auth("student"); // Redirect to login screen
}

void viewSchedule(student student_logged_in) {
    system("cls");
    printTitle("Weekly Schedule");
    Group groups[MAX_GROUPS];
    int groupCount = 0;
    int found = 0;

    // Initialize groups array to avoid uninitialized data
    memset(groups, 0, sizeof(groups));

    // Load schedules into groups
    int scheduleCount = loadSchedules(groups, &groupCount);
    printf("Schedule for Group %s:\n", student_logged_in.class);

    if (scheduleCount > 0) {
        for (int i = 0; i < groupCount; i++) {
            // Check if the group matches the student's class
            if (strcmp(groups[i].groupName, student_logged_in.class) == 0) {
                for (int j = 0; j < DAYS_OF_WEEK; j++) {
                    printf("%s:\n", (j == 0 ? "Monday" : j == 1 ? "Tuesday" : j == 2 ? "Wednesday" : j == 3 ? "Thursday" : "Friday"));
                    for (int k = 0; k < groups[i].courseCount[j]; k++) {
                        // Print course details for the day
                        printf("  %s - %s - Time: %s, Classroom: %s\n",
                               groups[i].courses[j][k].courseName, groups[i].courses[j][k].teacherName,
                               groups[i].courses[j][k].time, groups[i].courses[j][k].classroom);
                        found = 1;
                    }
                }
            }
        }
    }

    if (!found) {
        char ch[256];
        snprintf(ch,sizeof(ch),"No schedules loaded for Group %s.\n", student_logged_in.class);
        printError(ch);
    }

    printDivider();
}

void viewGrades(student student_logged_in) {
    system("cls");
    printTitle("Grades");
    int count;
    grade grades[MAX_GRADES];
    count = loadGrades(grades);
    int found = 0;
    printf("Grades for: %s %s\n", student_logged_in.first_name, student_logged_in.last_name);

    for (int i = 0; i < count; i++) {
        if (strcmp(grades[i].CIN, student_logged_in.cin) == 0) {
            printf("  Subject: %s, Grade: %.2f\n", grades[i].subject, grades[i].grade);
            found = 1;
        }
    }

    if (!found) {
        char ch[256];
        snprintf(ch,sizeof(ch),"No grades found for CIN: %s\n", student_logged_in.cin);
        printError(ch);
    }

    printDivider();
}

void viewAssignments(student student_logged_in) {
    system("cls");
    printTitle("Assignments");
    int count = 0, found = 0;
    Assignment assignments[MAX_ASSIGNMENTS];
    count = loadAssignments(assignments);

    printf("Assignments for %s:\n", student_logged_in.class);
    for (int i = 0; i < count; i++) {
        if (strcmp(assignments[i].className, student_logged_in.class) == 0) {
            printf("  %s - Due: %d-%d-%d\n", assignments[i].name,
                   assignments[i].dueDate.year, assignments[i].dueDate.month, assignments[i].dueDate.day);
            found = 1;
        }
    }

    if (!found) {
        printError("No assignments available!");
    }

    printDivider();
}

void submitAssignment(student student_logged_in) {
    system("cls");
    printTitle("Submit Assignment");
    char assignmentName[100];
    int count = 0, found = 0;
    Assignment assignments[MAX_ASSIGNMENTS];
    count = loadAssignments(assignments);
    char assign[256];
    printf("Enter the name of the assignment to submit: ");
    scanf(" %[^\n]s", assignmentName); // Allows spaces in input
    char date[10];
    getCurrentDate(date);

    for (int i = 0; i < count; i++) {
        if (strcmp(assignments[i].className, student_logged_in.class) == 0 &&
            strcmp(assignments[i].name, assignmentName) == 0) {
            snprintf(assign,sizeof(assign),"Assignment '%s' has been successfully submitted.\n", assignmentName);
            printSuccess(assign);
            found = 1;

            // Record the submission
            FILE *file = fopen("../data/submitted_assignments.txt", "a");
            if (file) {
                fprintf(file, "%s,%s,%s,%s,%s,%s\n", student_logged_in.class, student_logged_in.cin,
                        student_logged_in.first_name, student_logged_in.last_name, assignmentName,date);
                fclose(file);
            }
            break;
            }
    }

    if (!found) {
        printError("Assignment not found!");
    }

    printDivider();
}

void view_notifications(student student_logged_in) {
    system("cls");
    printTitle("Notifications");
    displayNotifications(student_logged_in.cin);
    printDivider();
}

void view_eliminations(student student_logged_in) {
    system("cls");
    printTitle("Eliminations");
    display_eliminations(student_logged_in);
    printDivider();
}

College* loadDoubleDiplomaColleges(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf(RED "Error opening file: %s\n" RESET, filename);
        return NULL;
    }

    College* head = NULL;
    College* tail = NULL;
    char line[256];

    // Skip the header line if the file has one
    fgets(line, sizeof(line), file);

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        College* newCollege = (College*)malloc(sizeof(College));
        if (!newCollege) {
            printf(RED "Memory allocation failed.\n" RESET);
            fclose(file);
            return head;  // Return the partially created list
        }

        // Parse the line
        if (sscanf(line, "%d,%99[^,],%49[^\n]", &newCollege->id, newCollege->name, newCollege->country) == 3) {
            newCollege->next = NULL;

            // Add to the linked list
            if (head == NULL) {
                head = tail = newCollege;
            } else {
                tail->next = newCollege;
                tail = newCollege;
            }
        } else {
            free(newCollege);  // Free memory if parsing fails
            printf(YELLOW "Invalid line format: %s" RESET, line);
        }
    }

    fclose(file);
    return head;
}

void freeCollegeList(College* head) {
    College* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void displayDoubleDiplomaColleges() {
    system("cls");
    // Load colleges from file
    College* head = loadDoubleDiplomaColleges("../data/colleges.txt");
    if (head == NULL) {
        printf(RED "No double diploma colleges available.\n" RESET);
        return;
    }

    // Display colleges
    printTitle("Double Diploma Colleges");
    College* current = head;
    while (current != NULL) {
        printf("ID: %d, Name: %s, Country: %s\n", current->id, current->name, current->country);
        current = current->next;
    }

    printDivider();

    // Free the linked list to avoid memory leaks
    freeCollegeList(head);
}

float calculateGradeThreshold(students* head) {
    int studentCount = 0;
    float totalStudentAverages = 0.0;
    grade grades[MAX_GRADES];
    int gradeCount = loadGrades(grades);

    students* current = head;
    while (current != NULL) {
        float totalGrades = 0.0;
        int subjectCount = 0;

        for (int i = 0; i < gradeCount; i++) {
            if (strcmp(current->student.cin, grades[i].CIN) == 0) {
                totalGrades += grades[i].grade;
                subjectCount++;
            }
        }

        if (subjectCount > 0) {
            float average = totalGrades / subjectCount;
            totalStudentAverages += average;
            studentCount++;
        }

        current = current->next;
    }

    return (studentCount == 0) ? 0.0 : totalStudentAverages / studentCount;
}

float studentGradeAverage(student* student_logged_in) {
    grade grades[MAX_GRADES];
    int count = loadGrades(grades);
    float gradeAverage = 0.0;
    int sub_count = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(grades[i].CIN, student_logged_in->cin) == 0) {
            gradeAverage += grades[i].grade;
            sub_count++;
        }
    }

    return (sub_count == 0) ? 0.0 : gradeAverage / sub_count;
}

int calculateAbsences(student* student_logged_in) {
    int count, abs = 0;
    absences records[MAX_ATTENDANCE_RECORDS];
    count = load_absences(records);

    for (int i = 0; i < count; i++) {
        if (strcmp(records[i].cin, student_logged_in->cin) == 0) {
            abs += records[i].absenceCount;
        }
    }

    return abs;
}

float calculateDDChance(student* student, students* head) {
    int abs = calculateAbsences(student);
    float average = studentGradeAverage(student);
    float gradeThreshold = calculateGradeThreshold(head);

    if (average >= gradeThreshold && abs <= 30) {
        return 100; // 100% chance
    }

    float gradeFactor = (gradeThreshold > 0) ? (average / gradeThreshold) : 0;
    float absenceFactor = 1.0 - ((float)abs / 30);

    return gradeFactor * absenceFactor * 100;
}

void calculateDDChanceUI(student* student, students* head) {
    system("cls");
    printTitle("DD Chances");
    float chance = calculateDDChance(student, head);
    printf("\nYour chance for a double diploma is: %.2f%%\n", chance);
    printDivider();
}

void student_ui(student student_logged_in, students* students_login) {
    int schoice;
    fflush(stdin);
    do {
        system("cls");
        printf(MOVE_CURSOR(0, 0));
        printTitle("Student Interface"); // Print the title

        // Displaying menu options
        printMenuOption(1, "View Personal Details", 1);
        printMenuOption(2, "View Schedule", 1);
        printMenuOption(3, "View Grades", 1);
        printMenuOption(4, "View Assignments", 1);
        printMenuOption(5, "Submit Assignment", 1);
        printMenuOption(6, "View Absences", 1);
        printMenuOption(7, "Double Diploma Options", 1);
        printMenuOption(8, "View Eliminations", 1);
        printMenuOption(9, "Check Notifications", 1);
        printMenuOption(0, "Logout", 1);
        getChoice(&schoice);

        switch (schoice) {
            case 1:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                viewPersonalDetails(student_logged_in, students_login);
                break;
            case 2:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                viewSchedule(student_logged_in);
                waitForUser();
                break;
            case 3:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                viewGrades(student_logged_in);
                waitForUser();
                break;
            case 4:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                viewAssignments(student_logged_in);
                waitForUser();
                break;
            case 5:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                submitAssignment(student_logged_in);
                waitForUser();
                break;
            case 6:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                view_absences(student_logged_in);
                break;
            case 7:
                system("cls");


                int ddchoice;

                do {
                    printTitle("Double Diploma");
                    printMenuOption(1,"View Double Diploma Colleges",1);
                    printMenuOption(2,"Calculate Double Diploma Chances",1);
                    printMenuOption(0,"Return",1);
                    getChoice(&ddchoice);
                    switch (ddchoice) {
                        case 1:
                            stopMP3();
                        playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                        sleep(1);
                            displayDoubleDiplomaColleges();
                        waitForUser();
                        break;
                        case 2:
                            stopMP3();
                        playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                        sleep(1);
                            calculateDDChanceUI(&student_logged_in, students_login);
                        waitForUser();
                        break;
                        case 0:
                            stopMP3();
                        playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                            printSuccess("Returning...");
                        sleep(1);
                        break;
                        default:
                            stopMP3();
                        playMP3("../sound/wrong-47985.mp3");
                            printError("Invalid Choice. Please try again.");
                        sleep(1);
                        break;

                    }
                }while (ddchoice != 0);
                break;
            case 8:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                view_eliminations(student_logged_in);
                waitForUser();
                break;
            case 9:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                view_notifications(student_logged_in);
                waitForUser();
                break;
            case 0:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                saveStudentsToFile(students_login, "../data/students.txt");
                logout();
                return;
            default:
                stopMP3();
            playMP3("../sound/wrong-47985.mp3");
                printError("Invalid choice. Please try again.");
            sleep(1);
            break;
        }
    } while (schoice != 0);
}
