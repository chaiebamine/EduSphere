#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <unistd.h>
#include "../ansi_codes.h"
#include "../utilities/utilities.h"
#include "../student/student_interface.h"
#include "../teacher/teacher_interface.h"
#include "../admins/admin_interface.h"
#include "../main_interface.h"

#include "auth.h"

void getPassword(char* password, int maxLength) {
    int index = 0;
    char ch;
    while ((ch = _getch()) != '\r' && index < maxLength - 1) {  // '\r' is Enter key
        if (ch == '\b' && index > 0) {  // Handle backspace
            printf("\b \b");  // Erase the last '*'
            index--;
        } else if (ch != '\b') {
            password[index++] = ch;
            printf("*");  // Display a star for each character
        }
    }
    password[index] = '\0';  // Null-terminate the string
    printf("\n");
}

// Function to handle login
void login(const char *role) {
    int logged_in = 0;
    char enteredUsername[50], enteredPassword[50];
    teachers *teachers_login = load_teachers_from_file("../data/teachers.txt");
    students *students_login = load_students_from_file("../data/students.txt");
    if (strcmp(role, "student") == 0) {

        printTitle("Login System");

        do {
            printf(BRIGHT_YELLOW"Enter username: "RESET);
            scanf("%s", enteredUsername);
            printf(BRIGHT_YELLOW"Enter password: "RESET);
            getPassword(enteredPassword, sizeof(enteredPassword));

            students *temp = students_login;
            logged_in = 0;  // Reset login state for each attempt

            // Validate credentials
            while (temp != NULL ) {
                if (strcmp(temp->student.username, enteredUsername) == 0 &&
                    strcmp(temp->student.password, enteredPassword) == 0) {
                    student student_logged_in = temp->student;
                    stopMP3();
                    playMP3("../sound/ff24f8ec1ecc43ba9e9608c07a8bd0b1.mp3");
                    printf(MOVE_FORWARD(40) BOLD GREEN "Login successful! Welcome, %s %s.\n" RESET,
                           student_logged_in.first_name, student_logged_in.last_name);

                    sleep(2);
                    student_ui(student_logged_in, students_login);
                    logged_in = 1;
                    break;  // Exit loop upon successful login
                    }
                temp = temp->next;
            }

            if (!logged_in) {
                stopMP3();
                playMP3("../sound/error-warning-login-denied-132113.mp3");
                printf(MOVE_FORWARD(40) BOLD RED "Invalid credentials. Please try again.\n" RESET);
            }

        } while (!logged_in);
    }else if (strcmp(role,"teacher")==0) {
        printTitle("Login System");
        do {
            printf(BRIGHT_YELLOW"Enter username: "RESET);
            scanf("%s", enteredUsername);
            printf(BRIGHT_YELLOW"Enter password: "RESET);
            getPassword(enteredPassword,sizeof(enteredPassword));
            teachers *temp =teachers_login;

            // Validate credentials
            while(temp != NULL && logged_in == 0) {
                if (strcmp(temp->teacher.username, enteredUsername) == 0 &&
                    strcmp(temp->teacher.password, enteredPassword) == 0) {
                    teacher teacher_logged_in = temp->teacher;
                    stopMP3();
                    playMP3("../sound/ff24f8ec1ecc43ba9e9608c07a8bd0b1.mp3");
                    printf(MOVE_FORWARD(40) BOLD GREEN "Login successful! Welcome, %s %s.%s\n",
                        teacher_logged_in.first_name,teacher_logged_in.last_name,RESET);
                    sleep(2);
                    logged_in=1;
                    teacher_ui(teacher_logged_in,teachers_login,students_login);
                    }
                else {
                    temp = temp->next;
                }
            }
            if (logged_in != 1) {
                stopMP3();
                playMP3("../sound/error-warning-login-denied-132113.mp3");
                printError("Invalid credentials. Please try again.");
            }
        }while(logged_in==0);
    }else if (strcmp(role,"admin")==0) {

        admins *admins_login = load_admins_from_file("../data/admins.txt");

        printTitle("Login System");
        do {
            printf(BRIGHT_YELLOW"Enter username: "RESET);
            scanf("%s", enteredUsername);
            printf(BRIGHT_YELLOW"Enter password: "RESET);
            getPassword(enteredPassword,sizeof(enteredPassword));
            admins *temp =admins_login;

            // Validate credentials
            while(temp != NULL && logged_in==0) {
                if (strcmp(temp->admin.username, enteredUsername) == 0 &&
                    strcmp(temp->admin.password, enteredPassword) == 0) {
                    admin admin_logged_in = temp->admin;
                    stopMP3();
                    playMP3("../sound/ff24f8ec1ecc43ba9e9608c07a8bd0b1.mp3");
                    printf(MOVE_FORWARD(40) BOLD GREEN "Login successful! Welcome, %s %s.%s\n",
                        admin_logged_in.first_name,admin_logged_in.last_name,RESET);
                    sleep(2);
                    logged_in=1;
                    admin_ui(admin_logged_in,admins_login,teachers_login,students_login);
                    }
                else {
                    temp = temp->next;
                }
            }
            if (logged_in != 1) {
                stopMP3();
                playMP3("../sound/error-warning-login-denied-132113.mp3");
                printError( "Invalid credentials. Please try again.");
            }
        }while(logged_in==0);
    }
}

void saveStudentsToFile(students *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Optional: Write the header to the file
    fprintf(file, "CIN,First Name,Last Name,Date of Birth,Gender,Email,Phone Number,Address,City,Class,Username,Password\n");

    students *current = head;
    while (current != NULL) {
        fprintf(file, "%s,%s,%s,%04d-%02d-%02d,%c,%s,%s,%s,%s,%s,%s,%s\n",
                current->student.cin,
                current->student.first_name,
                current->student.last_name,
                current->student.date_of_birth.year,
                current->student.date_of_birth.month,
                current->student.date_of_birth.day,
                current->student.gender,
                current->student.email,
                current->student.phone_number,
                current->student.address,
                current->student.city,
                current->student.class,
                current->student.username,
                current->student.password);
        current = current->next;
    }

    fclose(file);
}
void saveTeachersToFile(teachers *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Optional: Write the header to the file
    fprintf(file, "CIN,First Name,Last Name,Date of Birth,Gender,Email,Phone Number,Address,City,Department,Subject,Username,Password\n");

    teachers *current = head;
    while (current != NULL) {
        fprintf(file, "%s,%s,%s,%04d-%02d-%02d,%c,%s,%s,%s,%s,%s,%s,%s,%s\n",
                current->teacher.cin,
                current->teacher.first_name,
                current->teacher.last_name,
                current->teacher.date_of_birth.year,
                current->teacher.date_of_birth.month,
                current->teacher.date_of_birth.day,
                current->teacher.gender,
                current->teacher.email,
                current->teacher.phone_number,
                current->teacher.address,
                current->teacher.city,
                current->teacher.departement,
                current->teacher.subject,
                current->teacher.username,
                current->teacher.password);
        current = current->next;
    }

    fclose(file);
}

void saveAdminsToFile(admins *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Optional: Write the header to the file
    fprintf(file, "CIN,First Name,Last Name,Date of Birth,Gender,Email,Phone Number,Address,City,Role,Username,Password\n");

    admins *current = head;
    while (current != NULL) {
        fprintf(file, "%s,%s,%s,%04d-%02d-%02d,%c,%s,%s,%s,%s,%s,%s,%s\n",
                current->admin.cin,
                current->admin.first_name,
                current->admin.last_name,
                current->admin.date_of_birth.year,
                current->admin.date_of_birth.month,
                current->admin.date_of_birth.day,
                current->admin.gender,
                current->admin.email,
                current->admin.phone_number,
                current->admin.address,
                current->admin.city,
                current->admin.role,
                current->admin.username,
                current->admin.password);
        current = current->next;
    }

    fclose(file);
}


void initializeQueue(WaitlistQueue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
}
void enqueue(WaitlistQueue* queue, student new_student) {
    Waitlist* new_node = (Waitlist*)malloc(sizeof(Waitlist));
    if (!new_node) {
        printf("Memory allocation failed.\n");
        return;
    }

    new_node->data = new_student;
    new_node->next = NULL;

    if (queue->rear == NULL) {
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
}

void saveWaitlistToFile(WaitlistQueue* queue, const char* filename) {
    FILE* file = fopen(filename, "w");  // Open in append mode
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }
    fprintf(file, "CIN,First Name,Last Name,Date of Birth,Gender,Email,Phone Number,Address,City,Class,Username,Password\n");

    Waitlist* current = queue->front;
    while (current != NULL) {
        fprintf(file, "%s,%s,%s,%04d-%02d-%02d,%c,%s,%s,%s,%s,%s,%s,%s\n",
                current->data.cin,
                current->data.first_name,
                current->data.last_name,
                current->data.date_of_birth.year,
                current->data.date_of_birth.month,
                current->data.date_of_birth.day,
                current->data.gender,
                current->data.email,
                current->data.phone_number,
                current->data.address,
                current->data.city,
                "waitlist",
                current->data.username,
                current->data.password);  // Class left empty
        current = current->next;
    }

    fclose(file);
}
void loadWaitlistFromFile(WaitlistQueue* queue, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file or file not found.\n");
        return;
    }

    char line[512];  // Buffer to hold each line
    fgets(line,sizeof(line),file);
    while (fgets(line, sizeof(line), file)) {
        student new_student;

        // Parse the student data from the line (assuming CSV format)
        sscanf(line, "%8[^,],%29[^,],%29[^,],%d-%d-%d,%c,%49[^,],%49[^,],%99[^,],%29[^,],%9[^,],%99[^,],%99[^\n]",
               new_student.cin,
               new_student.first_name,
               new_student.last_name,
               &new_student.date_of_birth.year,
               &new_student.date_of_birth.month,
               &new_student.date_of_birth.day,
               &new_student.gender,
               new_student.email,
               new_student.phone_number,
               new_student.address,
               new_student.city,
               new_student.class,
               new_student.username,
               new_student.password);

        // Enqueue the student into the waitlist
        enqueue(queue, new_student);
    }

    fclose(file);
}

void subscribe(WaitlistQueue* queue) {
    student new_student;
    students *students_list = load_students_from_file("../data/students.txt");
    int valid = 0;
    do {
        printf(BRIGHT_YELLOW "Enter CIN: "RESET);
        scanf("%8s", new_student.cin);
        valid = validateCIN(new_student.cin) && studentExistsByCIN(students_list,new_student.cin) && studentExistsInQueue(queue,new_student.cin);
    }while (valid == 0);
    printf(BRIGHT_YELLOW"Enter First Name: "RESET);
    scanf("%29s", new_student.first_name);
    printf(BRIGHT_YELLOW"Enter Last Name: "RESET);
    scanf("%29s", new_student.last_name);
    do {
        printf(BRIGHT_YELLOW"Enter Date of Birth (YYYY-MM-DD): "RESET);
        scanf("%d-%d-%d", &new_student.date_of_birth.year, &new_student.date_of_birth.month, &new_student.date_of_birth.day);
        if (!isValidDate(new_student.date_of_birth)) {
            printf(RED"Invalid date. Please try again.\n"RESET);
        }
    }while (!isValidDate(new_student.date_of_birth));
    printf(BRIGHT_YELLOW"Enter Gender (M/F): "RESET);
    scanf(" %c", &new_student.gender);
    do {
        fflush(stdin);
        printf(BRIGHT_YELLOW"Enter Email: "RESET);
        scanf("%[^\n]s", new_student.email);
    }while (!validateEmail(new_student.email));
    printf(BRIGHT_YELLOW"Enter Phone Number: "RESET);
    scanf("%49s", new_student.phone_number);
    printf(BRIGHT_YELLOW"Enter Address: "RESET);
    scanf(" %[^\n]s", new_student.address);
    printf(BRIGHT_YELLOW"Enter City: "RESET);
    fflush(stdin);
    scanf("%29s", new_student.city);
    printf(BRIGHT_YELLOW"Enter Username: "RESET);
    scanf("%99s", new_student.username);
    do {
        printf(BRIGHT_YELLOW"Enter Password: "RESET);
        scanf("%99s", new_student.password);
    }while (!validatePassword(new_student.password));

    strcpy(new_student.class, "waitlist");  // Leave class empty until approval
    enqueue(queue, new_student);
    printf("%s %s saved to waitlist",new_student.first_name,new_student.last_name);
}

void auth(const char *role) {
    int choice;
    const char *ActionS[] = {"1. Login", "2. Register", "0. Exit"};
    const char *Action[] = {"1. Login","0. Exit"};
    char ch[256 ];
    snprintf(ch,sizeof(ch),"\r %s %s ===Authentification",CLEAR_FROM_CURSOR,MOVE_FORWARD(40));
    printTitle(ch);
    do {
        if (strcmp(role,"student")==0) {
            printButtons(3,ActionS);
        }else {
            printButtons(2,Action);
        }
        fflush(stdin);
        // Reading input safely using fgets
        getChoice(&choice);
        switch (choice) {
            case 1:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                system("cls");

                printf(CLEAR_SCREEN MOVE_CURSOR(0,0));
                login(role);
                return;  // Exit the loop after a valid choice
            case 2:

                if (strcmp(role,"student")==0) {
                    stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    printf(MOVE_FORWARD(50) GREEN BOLD "Register" RESET);
                    WaitlistQueue queue;
                    initializeQueue(&queue);

                    // Load existing waitlist from the file
                    loadWaitlistFromFile(&queue, "../data/registration.txt");
                    subscribe(&queue);
                    saveWaitlistToFile(&queue,"../data/registration.txt");
                    sleep(2); // sleep for 2 seconds
                    waitForUser();
                    auth(role);
                }else {
                    stopMP3();
                    playMP3("../sound/wrong-47985.mp3");
                    printError("Invalid choice. Please select a valid option (1 or 0 to exit).");
                    continue;
                }
            case 0:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                printSuccess("Goodbye");
                sleep(1);
            system("cls");
                printf(CLEAR_SCREEN MOVE_CURSOR(0,0));
                main_ui();
                  // Exit the loop and terminate
            default:
                stopMP3();
            playMP3("../sound/wrong-47985.mp3");
                printError("Invalid choice. Please select a valid option (1 ,2 or 0 to exit).");
                break;  // Continue the loop if the choice is invalid
        }

    } while (1); // Continue looping until a valid choice is made
}

