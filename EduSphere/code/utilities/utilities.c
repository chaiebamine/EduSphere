#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>
#include "../ansi_codes.h"
#include "utilities.h"



void playMP3(const char *filePath) {
    // Stop any previously playing sound
    stopMP3();

    // Build and execute the play command
    char command[256];
    snprintf(command, sizeof(command), "open \"%s\" type mpegvideo alias sound", filePath);
    if (mciSendString(command, NULL, 0, NULL) != 0) {
        printf("Error: Unable to open sound file: %s\n", filePath);
        return;
    }
    mciSendString("play sound", NULL, 0, NULL);
}

void stopMP3() {
    // Stop and close the currently playing sound
    mciSendString("stop sound", NULL, 0, NULL);
    mciSendString("close sound", NULL, 0, NULL);
}

// Utility function to validate input is numeric
int isValidNumber(const char *input) {
    for (int i = 0; input[i] != '\0'; i++) {
        // Allow digits only and handle trailing newline or null terminator
        if (!isdigit(input[i]) && input[i] != '\n') {
            return 0; // Invalid character found
        }
    }
    return 1;
}

void getChoice(int *choice) {
    fflush(stdin);
    char input[10];  // Buffer for user input
    while (1) {
        printf(BRIGHT_YELLOW "Enter your choice: " RESET);
        fgets(input, sizeof(input), stdin);  // Read user input as a string

        // Remove newline character from input (if present)
        input[strcspn(input, "\n")] = '\0';

        // Check if input is empty
        if (strlen(input) == 0) {
            simulateLoading(1, "loading");

            // Play error sound for empty input
            stopMP3();
            playMP3("../sound/wrong-47985.mp3");

            // Show error message for empty input
            printf(VISIBLE);
            printError("Input cannot be empty. Please enter a valid number." );
            sleep(1);  // Pause briefly before prompting again
            continue;
        }

        // Validate if input is a valid number
        if (isValidNumber(input)) {
            simulateLoading(1, "loading");
            printf(VISIBLE);

            // Convert valid input to integer and exit loop
            *choice = atoi(input);
            break;
        } else {
            simulateLoading(1, "loading");

            // Play error sound for invalid input
            stopMP3();
            playMP3("../sound/wrong-47985.mp3");

            // Show error message for invalid input
            printf(VISIBLE);
            printError("Invalid input. Please enter a valid number.");
            sleep(1);  // Pause briefly before prompting again
        }
    }
}

void animatedText(const char* header) {
    for (int i = 0; header[i] != '\0'; i++) {
        printf("%c", header[i]);
        fflush(stdout);
        usleep(10000); // 0.1-second delay
    }
}

void printMenuOption(int optionNumber, const char* description, int isActive) {
    char option[256];
    if (isActive) {
        snprintf(option, sizeof(option), "\033[1;34m[%d] %s\033[0m\n", optionNumber, description);
        animatedText(option);// Blue for active
    } else {
        snprintf(option, sizeof(option), BRIGHT_BLACK"[%d] %s\n"RESET, optionNumber, description);
        animatedText(option);
    }
}

void printTitle(const char* title) {
    printf(TITLE_COLOR "\n=== %s ===\n" RESET, title);
}

void printDivider() {
    printf("\n============================================================\n");
}
void printSuccess(const char* message) {
    printf(MOVE_FORWARD(35) SUCCESS_COLOR "%s\n" RESET, message);
}

void printError(const char* message) {
    printf(MOVE_FORWARD(35)  ERROR_COLOR "%s\n" RESET, message);
}

// Function to create a new student node
students* create_student(student s) {
    students *new_student = (students *)malloc(sizeof(students));
    new_student->student = s;
    new_student->next = NULL;
    return new_student;
}

// Function to add a student to the linked list
void add_student(students **head, students *new_student) {
    new_student->next = *head;
    *head = new_student;
}

// Function to create a new student node
teachers* create_teacher(teacher t) {
    teachers *new_teacher = (teachers *)malloc(sizeof(teachers));
    new_teacher->teacher = t;
    new_teacher->next = NULL;
    return new_teacher;
}

// Function to add a student to the linked list
void add_teacher(teachers **head, teachers *new_teacher) {
    new_teacher->next = *head;
    *head = new_teacher;
}

// Function to create a new student node
admins* create_admin(admin a) {
    admins *new_admin = (admins *)malloc(sizeof(admins));
    new_admin->admin = a;
    new_admin->next = NULL;
    return new_admin;
}

// Function to add a student to the linked list
void add_admin(admins **head, admins *new_admin) {
    new_admin->next = *head;
    *head = new_admin;
}

// Function to load students from a CSV file and store them in a linked list
students* load_students_from_file(const char *filename) {
    students *head = NULL;
    FILE *file = fopen(filename, "r");
    if (file) {
        char line[512];

        // Skip header line if present
        fgets(line, sizeof(line), file); // Assuming the first line is a header

        // Read each line and parse it into the student struct using sscanf
        while (fgets(line, sizeof(line), file)) {
            student s;

            // Parse the line into the student struct
            sscanf(line, "%8[^,],%29[^,],%29[^,],%d-%d-%d,%c,%49[^,],%49[^,],%99[^,],%29[^,],%9[^,],%99[^,],%99s",
                   s.cin, s.first_name, s.last_name,
                   &s.date_of_birth.year, &s.date_of_birth.month, &s.date_of_birth.day,
                    &s.gender, s.email, s.phone_number,
                   s.address, s.city, s.class,s.username,s.password);
            students *new_student = create_student(s);

            // Add the new student to the linked list
            add_student(&head, new_student);
        }


        fclose(file);
    } else {
        printf("Error opening file.\n");
    }
    return head;
}

void waitForUser() {
    fflush(stdin);
    printf("\nPress enter to return...");
    getchar();
    printf(CLEAR_SCREEN MOVE_CURSOR(0,0));
}

teachers* load_teachers_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Unable to open file %s.\n", filename);
        return NULL;
    }
    else {
        teachers *head = NULL;
        // Buffer to store each line of the file
        char line[512];
        // Skip the header line
        fgets(line, sizeof(line), file);

        // Read each line of the file
        while (fgets(line, sizeof(line), file)) {
            teacher t;
            char dob[11]; // Temporary buffer for date of birth (YYYY-MM-DD)

            // Parse the line
            sscanf(line, "%8[^,],%29[^,],%29[^,],%10[^,],%c,%49[^,],%49[^,],%99[^,],%29[^,],%49[^,],%49[^,],%99[^,],%99[^\n]",
                   t.cin, t.first_name, t.last_name, dob, &t.gender,
                   t.email, t.phone_number, t.address, t.city,t.departement ,t.subject,
                   t.username, t.password);

            // Parse the date of birth
            sscanf(dob, "%d-%d-%d", &t.date_of_birth.year, &t.date_of_birth.month, &t.date_of_birth.day);

            // Create a new teacher node
            teachers *new_teacher = create_teacher(t);
            // add teacher node
            add_teacher(&head, new_teacher);
        }

        fclose(file);
        return head;
    }
}

// Function to load students from a CSV file and store them in a linked list
admins* load_admins_from_file(const char *filename) {
    admins *head = NULL;
    FILE *file = fopen(filename, "r");
    if (file) {
        char line[512];

        // Skip header line if present
        fgets(line, sizeof(line), file); // Assuming the first line is a header

        // Read each line and parse it into the student struct using sscanf
        while (fgets(line, sizeof(line), file)) {
            admin a;

            // Parse the line into the student struct
            sscanf(line, "%8[^,],%29[^,],%29[^,],%d-%d-%d,%c,%49[^,],%49[^,],%99[^,],%29[^,],%9[^,],%99[^,],%99s",
                   a.cin, a.first_name, a.last_name,
                   &a.date_of_birth.year, &a.date_of_birth.month, &a.date_of_birth.day,
                    &a.gender, a.email, a.phone_number,
                   a.address, a.city, a.role,a.username,a.password);
            admins *new_admin = create_admin(a);

            // Add the new student to the linked list
            add_admin(&head, new_admin);
        }


        fclose(file);
    } else {
        printf("Error opening file.\n");
    }
    return head;
}

void printTitleBox() {
    printf(MOVE_FORWARD(30)"  ______    _        _____       _                   \n"
            MOVE_FORWARD(30)" |  ____|  | |      / ____|     | |                  \n"
            MOVE_FORWARD(30)" | |__   __| |_   _| (___  ____ | |__   ___ ____ ___ \n"
            MOVE_FORWARD(30)" |  __| / _  | | | |\\___ \\|  _ \\|  _ \\ / _ \\  __/ _ \\ \n"
            MOVE_FORWARD(30)" | |___| (_| | |_| |____) | |_) | | | |  __/ | |  __/ \n"
            MOVE_FORWARD(30)" |______\\____|\\____|_____/| .__/|_| |_|\\___|_|  \\___| \n"
            MOVE_FORWARD(30)"                          | |                         \n"
            MOVE_FORWARD(30)"                          |_|                         \n"
     );

}

void printBox(int n,const char *labels[]) {
    // Simulating a button with box-like structure
    int sizes[10];
    for(int i = 0; i < n; i++) {
        sizes[i] = strlen(labels[i])+2;
    }
    if (n==2) {
        printf("\n");
        printf(MOVE_FORWARD(17));
    }else if (n==3) {
        printf("\n");
        printf(MOVE_FORWARD(8));
    }else {
        printf("\n");
    }
    printf(MOVE_FORWARD(25));
    for(int j=0;j<n;j++) {
        printf("+");
        for(int i = 0; i < sizes[j]; i++) {
            printf("-");
        }
        printf("+");
        printf(MOVE_FORWARD(4));
    }
    if (n==2) {
        printf("\n");
        printf(MOVE_FORWARD(17));
    }else if (n==3) {
        printf("\n");
        printf(MOVE_FORWARD(8));
    }else {
        printf("\n");
    }
    printf(MOVE_FORWARD(25));
    for(int j=0;j<n;j++) {
        printf("| %-s |" ,labels[j]);
        printf(MOVE_FORWARD(4));
    }
    if (n==2) {
        printf("\n");
        printf(MOVE_FORWARD(17));
    }else if (n==3) {
        printf("\n");
        printf(MOVE_FORWARD(8));
    }else {
        printf("\n");
    }
    printf(MOVE_FORWARD(25));
    for(int j=0;j<n;j++) {
        printf("+");
        for(int i = 0; i < sizes[j]; i++) {
            printf("-");
        }
        printf("+");
        printf(MOVE_FORWARD(4));
    }
    printf("\n");
}

void printButtons(int n,const char *labels[]) {
    // Simulating a button with box-like structure

    printf("\n");
    printBox(n,labels);
    printf("\n");
}

// Utility Function: Validate CIN
int validateCIN(const char* cin) {
    if (strlen(cin) != 8) {
        printf(RED "Invalid CIN: Must be 8 characters.\n" RESET);
        return 0;
    }
    for (int i = 0; i < 8; i++) {
        if (!isdigit(cin[i])) {
            printf(RED "Invalid CIN: Must contain only numbers.\n" RESET);
            return 0;
        }
    }
    return 1;
}

// Utility Function: Validate Password
int validatePassword(const char* password) {
    if (strlen(password) < 6) {
        printf(RED "Invalid Password: Must be at least 6 characters long.\n" RESET);
        return 0;
    }
    return 1;
}

int validateEmail(const char* email) {
    // Ensure email is not null or empty
    if (email == NULL || strlen(email) == 0) {
        printf(RED"Error: Email cannot be empty.\n"RESET);
        return 0;
    }

    // Look for '@' symbol
    const char* at = strchr(email, '@');
    if (at == NULL) {
        printf(RED"Error: Email must contain an '@' symbol.\n"RESET);
        return 0; // No '@' found
    }
    if (at == email) {
        printf(RED"Error: Email cannot start with '@'.\n"RESET);
        return 0; // Starts with '@'
    }

    // Ensure there's a domain after '@'
    const char* domain = at + 1;
    if (*domain == '\0') {
        printf(RED"Error: Email must have a domain after '@'.\n"RESET);
        return 0; // No domain after '@'
    }
    if (strchr(domain, '.') == NULL) {
        printf(RED"Error: Domain must contain at least one '.' (e.g., example.com).\n"RESET);
        return 0; // No '.' in the domain
    }

    // Validate characters before '@'
    for (const char* ptr = email; ptr < at; ptr++) {
        if (!isalnum(*ptr) && *ptr != '.' && *ptr != '_' && *ptr != '-') {
            printf(RED"Error: Invalid character '%c' in the local part of the email.\n"RESET, *ptr);
            return 0; // Invalid character in local part
        }
    }

    // Validate domain (only alphanumeric, '-' and '.')
    int dotCount = 0;
    for (const char* ptr = domain; *ptr != '\0'; ptr++) {
        if (!isalnum(*ptr) && *ptr != '.' && *ptr != '-') {
            printf(RED"Error: Invalid character '%c' in the domain.\n"RESET, *ptr);
            return 0; // Invalid character in domain
        }
        if (*ptr == '.') {
            dotCount++;
        }
    }

    // Ensure domain has at least one dot
    if (dotCount < 1) {
        printf(RED"Error: Domain must contain at least one '.' (e.g., example.com).\n"RESET);
        return 0;
    }

    // If all checks pass
    return 1; // Email is valid
}

int studentExistsInQueue(WaitlistQueue* queue, const char* cin) {
    if (queue == NULL || queue->front == NULL) {
        return 1; // Queue is empty
    }

    Waitlist* current = queue->front;
    while (current != NULL) {
        if (strcmp(current->data.cin, cin) == 0) {
            printf(RED"CIN already exists\n");
            return 0; // CIN found in the waitlist
        }
        current = current->next; // Move to the next node
    }

    return 1; // CIN not found
}


int studentExistsByCIN(students* head, const char* cin) {
    while (head != NULL) {
        if (strcmp(head->student.cin, cin) == 0) {
            printf(RED"CIN already exists\n");
            return 0; // Student found
        }
        head = head->next; // Move to the next node
    }
    return 1; // Student not found
}

int teacherExistsByCIN(teachers* head, const char* cin) {
    while (head != NULL) {
        if (strcmp(head->teacher.cin, cin) == 0) {
            printf(RED"CIN exists\n");
            return 0; // Teacher found
        }
        head = head->next; // Move to the next node
    }
    return 1; // Teacher not found
}

int adminExistsByCIN(admins* head, const char* cin) {
    while (head != NULL) {
        if (strcmp(head->admin.cin, cin) == 0) {
            printf(RED"CIN already exists\n");
            return 0; // admin found
        }
        head = head->next; // Move to the next node
    }
    return 1; // Admin not found
}

void simulateProgressBar(int duration) {
    int i;
    int total = 50;  // Total length of the progress bar (50 characters)
    printf(HIDE_CURSOR);
    int percentage = 0;
    const char* color;
    for (i = 0; i <= total; i++) {
        percentage = (i * 100) / total;
        color = (percentage < 35) ? "\033[1;31m" // Red
                          : (percentage < 65) ? "\033[1;33m" // Yellow
                          : "\033[1;32m";                  // Green
        printf("\r\033[1;37m║%s%s",BG_BRIGHT_BLACK,color);
        for (int j = 0; j < total; j++) {
            if (j < i) {
                printf("█");  // Filled part
            } else {
                printf(" ");  // Empty part
            }
        }
        printf(RESET"\033[1;37m║\033[0m %d%%", percentage);  // Percentage
        fflush(stdout);  // Ensure the output is displayed immediately
        usleep((duration * 1000000) / total);  // Adjust speed based on duration
    }
    printf(SHOW_CURSOR"\n");
}

void simulateLoading(int duration,const char *message) {
    // This function will simulate a loading animation for a given duration in seconds
    int i;
    char loadingMessage[50];
    strcpy(loadingMessage, message);


    printf(HIDE_CURSOR);
    printf(MOVE_FORWARD(50) "%s", loadingMessage);
    // Loop for a given duration to simulate loading
    for (i = 0; i < duration * 6; i++) {
        // Print "Loading" with some dots
         // \r returns the cursor to the start of the line

        // Add dots for loading effect
        switch (i % 3) {
            case 0:
                printf(".");
                printf(MOVE_BACKWARD(1));

            break;
            case 1:
                printf("..");
                printf(MOVE_BACKWARD(2));

            break;
            case 2:
                printf("...");
                printf(MOVE_BACKWARD(3));

            break;
        }

        fflush(stdout);  // Ensure the output is displayed immediately

        // Wait for 100ms (simulating the loading time)
        usleep(500000);  // 100000 microseconds = 0.1 seconds
        printf(CLEAR_FROM_CURSOR);
    }

    printf("\r"CLEAR_FROM_CURSOR);
    printf(SHOW_CURSOR);

}

// Function to load assignments from a file
int loadAssignments(Assignment assignments[]) {
    FILE *file = fopen("../data/assignments.txt", "r");
    if (!file) {
        printf("Error opening file.\n");
        return 0;
    }

    int count = 0;
    char line[512];  // Buffer to hold each line

    // Skip the first line (header) if necessary
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        // Parse each line: Class Name, Assignment Name, Due Date (YYYY-MM-DD)
        if (sscanf(line, " %49[^,], %99[^,], %d-%d-%d",
                    assignments[count].className, assignments[count].name,
                    &assignments[count].dueDate.year, &assignments[count].dueDate.month, &assignments[count].dueDate.day) == 5)
            count++;
    }

    fclose(file);
    return count;
}

// Function to load schedules from a file
int loadSchedules(Group groups[], int* groupCount) {
    FILE *file = fopen("../data/schedules.txt", "r");
    if (!file) {
        printf("Error opening file.\n");
        return 0;
    }

    int count = 0;
    char line[512];  // Buffer to hold each line
    fgets(line,sizeof(line),file);
    // Read each line from the file
    while (fgets(line, sizeof(line), file)) {
        // Extract group name, day, course, teacher, time, classroom
        char groupName[10], day[20], courseName[50], teacherName[50], time[20], classroom[50];

        if (sscanf(line, "%9[^,], %19[^,], %49[^,], %49[^,], %19[^,], %49[^\n]",
                    groupName, day, courseName, teacherName, time, classroom) == 6) {
            int groupIndex = -1;
            for (int i = 0; i < count; i++) {
                if (strcmp(groups[i].groupName, groupName) == 0) {
                    groupIndex = i;
                    break;
                }
            }

            // If the group doesn't exist, create a new one
            if (groupIndex == -1) {
                strcpy(groups[count].groupName, groupName);
                groupIndex = count++;
            }

            // Find the day index (e.g., Monday -> 0, Tuesday -> 1, etc.)
            int dayIndex = -1;
            const char* days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
            for (int i = 0; i < 5; i++) {
                if (strcmp(day, days[i]) == 0) {
                    dayIndex = i;
                    break;
                }
            }

            // Add the course to the corresponding day and session (morning or afternoon)
            if (dayIndex >= 0) {
                int courseIdx = groups[groupIndex].courseCount[dayIndex]++;
                strcpy(groups[groupIndex].courses[dayIndex][courseIdx].courseName, courseName);
                strcpy(groups[groupIndex].courses[dayIndex][courseIdx].teacherName, teacherName);
                strcpy(groups[groupIndex].courses[dayIndex][courseIdx].time, time);
                strcpy(groups[groupIndex].courses[dayIndex][courseIdx].classroom, classroom);
            }
        }
    }

    fclose(file);
    *groupCount = count;
    return count;
}

// Function to load grades from a CSV file into an array of Grades structures
int loadGrades(grade grades[]) {
    FILE* file = fopen("../data/grades.txt", "r");
    if (!file) {
        printf("Error opening file.\n");
        return 0;
    }

    char line[100];
    int count = 0;

    // Skip the header line
    fgets(line, sizeof(line), file);

    // Read each line and populate the Grades array
    while (fgets(line, sizeof(line), file) && count < MAX_GRADES) {
        sscanf(line, "%8[^,],%19[^,],%f", grades[count].CIN, grades[count].subject, &grades[count].grade);
        count++;
    }

    fclose(file);
    return count;
}

// Function to save assignments back to file
void saveAssignments(const char* filename, Assignment assignments[], int count) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Unable to open assignments file for writing.\n");
        return;
    }
    fprintf(file,"Group,assignment,due_date\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%04d-%02d-%02d\n",
                assignments[i].className,
                assignments[i].name,
                assignments[i].dueDate.year,
                assignments[i].dueDate.month,
                assignments[i].dueDate.day);
    }

    fclose(file);
}

void saveGradesToFile(const char* filename, grade grades[], int gradeCount) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Unable to open file %s for writing.\n", filename);
        return;
    }

    // Write the header
    fprintf(file, "CIN,Subject,Grade\n");

    // Write each grade
    for (int i = 0; i < gradeCount; i++) {
        fprintf(file, "%s,%s,%.2f\n", grades[i].CIN, grades[i].subject, grades[i].grade);
    }

    fclose(file);
}

// Function to get the current date in YYYY-MM-DD format
void getCurrentDate(char* dateBuffer) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(dateBuffer, 11, "%Y-%m-%d", tm_info);
}

int loadTeacherSchedules(const char* filename, TeacherScheduleNode** head) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file.\n");
        return 0;
    }

    char line[512];
    int count = 0;

    // Skip the header line
    fgets(line, sizeof(line), file);

    // Read each line of the file
    while (fgets(line, sizeof(line), file)) {
        TeacherSchedule schedule;

        // Extract teacher schedule details
        if (sscanf(line, "%8[^,], %49[^,], %19[^,], %19[^,], %49[^,], %9[^\n]",
                    schedule.cin, schedule.subject, schedule.day, schedule.time, schedule.classroom, schedule.group) == 6) {

            // Create a new schedule node and add it to the list
            TeacherScheduleNode* newNode = (TeacherScheduleNode*)malloc(sizeof(TeacherScheduleNode));
            if (!newNode) {
                printf("Memory allocation failed.\n");
                fclose(file);
                return 0;
            }

            newNode->schedule = schedule;
            newNode->next = *head;
            *head = newNode;  // Add to the front of the list
            count++;
                    }
    }

    fclose(file);
    return count;
}

int loadSubmissions(const char* filename, Submission submissions[]) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Unable to open submissions file.\n");
        return 0;
    }

    char line[256];
    int count = 0;

    // Read each line from the file
    while (fgets(line, sizeof(line), file) && count < maxSubmissions) {
        sscanf(line, "%9[^,],%8[^,],%29[^,],%29[^,],%49[^,],%d-%d-%d",
               submissions[count].groupName,
               submissions[count].cin,
               submissions[count].firstName,
               submissions[count].lastName,
               submissions[count].assignmentName,
               &submissions[count].submissionDate.year,&submissions[count].submissionDate.month,&submissions[count].submissionDate.day);
        count++;
    }

    fclose(file);
    return count;
}

// Function to validate a date
int isValidDate(date d) {
    if (d.year < 1970 || d.year > 2024) return 0;
    if (d.month < 1 || d.month > 12) return 0;
    if (d.day < 1 || d.day > 31) return 0;

    // Handle months with fewer days
    if (d.month == 4 || d.month == 6 || d.month == 9 || d.month == 11) {
        if (d.day > 30) return 0;
    } else if (d.month == 2) {
        // Check for leap year
        int isLeapYear = (d.year % 4 == 0 && d.year % 100 != 0) || (d.year % 400 == 0);
        if (d.day > (isLeapYear ? 29 : 28)) return 0;
    }

    return 1;  // Valid date
}

// Function to compare two dates
int isDatePassed(date d1, date d2) {
    if (d1.year > d2.year) return 1;
    if (d1.year == d2.year && d1.month > d2.month) return 1;
    if (d1.year == d2.year && d1.month == d2.month && d1.day > d2.day) return 1;
    return 0;
}

// Function to remove expired assignments
void removeExpiredAssignments() {
    Assignment assignments[MAX_ASSIGNMENTS];
    int count = loadAssignments(assignments);

    if (count == 0) {
        return;
    }
    char Date[11];
    getCurrentDate(Date);
    date currentDate;
    sscanf(Date,"%4d-%2d-%2d",&currentDate.year,&currentDate.month,&currentDate.day);

    // Filter out expired assignments
    Assignment validAssignments[MAX_ASSIGNMENTS];
    int validCount = 0;

    for (int i = 0; i < count; i++) {
        if (!isDatePassed(currentDate, assignments[i].dueDate)) {
            validAssignments[validCount++] = assignments[i];  // Keep valid assignments
        } else {
            printf("Assignment '%s' for group '%s' has expired and will be removed.\n",
                   assignments[i].name, assignments[i].className);
        }
    }

    // Save only valid assignments back to the file
    saveAssignments("../data/assignments.txt", validAssignments, validCount);
}

