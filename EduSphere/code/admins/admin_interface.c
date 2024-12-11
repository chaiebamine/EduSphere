#include "../utilities/utilities.h"
#include "../authentification/auth.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../ansi_codes.h"
#include "admin_interface.h"
// Function to print the table header
void StudentTableHeader() {
    printf(BRIGHT_BLACK"-----------------------------------------------------------"
           "---------------------------------------------------------------------\n"RESET);
    printf("|  %sCIN   %s|%sFirst Name%s| %sLast Name %s|%sBirth Date%s|%sGender%s|"
           "             %sEmail            %s|%sPhone Number%s|        %sAddress       %s|    %sCity    %s|%sClass%s|\n",
           CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET);
    printf(BRIGHT_BLACK"-------------------------------------------------------------"
                       "-------------------------------------------------------------------\n"RESET);
}

// Function to print a single student's row in the table
void StudentRow(const student *s) {
    printf("|%-8s|%-10s|%-11s|%04d-%02d-%02d|%-6c|%-30s|%-12s|%-22s|%-12s|%-5s|\n",
           s->cin, s->first_name, s->last_name,
           s->date_of_birth.year, s->date_of_birth.month, s->date_of_birth.day,
           s->gender, s->email, s->phone_number,
           s->address, s->city, s->class);
    printf(BRIGHT_BLACK"------------------------------------------------------------------------"
                       "--------------------------------------------------------\n"RESET);
}

// Function to show the list of students
void show_students_info(students* students) {
    system("cls");
    printTitle("The list of students");



    // Debug: Check if the list is empty
    if (students == NULL) {
        printf("No students found.\n");
        return;
    }

    // Print the table header
    StudentTableHeader();

    // Traverse the linked list and print each student's data
    while (students != NULL) {
        StudentRow(&students->student);
        students = students->next;
    }
    printDivider();
}

// Function to display the waitlist
void view_waitlist(WaitlistQueue* waitlist) {
    if (!waitlist || !waitlist->front) {
        printf("Waitlist is empty.\n");
        return;
    }

    printTitle("Student Waitlist");
    Waitlist* temp = waitlist->front;
    while (temp) {
        printf("CIN: %s, Name: %s %s, Class: %s\n", temp->data.cin, temp->data.first_name, temp->data.last_name, temp->data.class);
        temp = temp->next;
    }
    printDivider();
}

// Function to update the waitlist file after modifying the waitlist
void updateWaitlistFile(WaitlistQueue* waitlist) {
    if (!waitlist) return;

    FILE* file = fopen("../data/registration.txt", "w");
    if (!file) {
        printf("Error opening waitlist file for writing.\n");
        return;
    }
    fprintf(file,"CIN,First Name,Last Name,Date of Birth,Gender,Email,Phone Number,Address,City,Class,Username,Password\n");
    // Iterate through the waitlist and write each student to the file
    Waitlist* current = waitlist->front;
    while (current) {
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
                current->data.class,
                current->data.username,
                current->data.password);
        current = current->next;
    }

    fclose(file);
}

void addStudent(WaitlistQueue* waitlist, students** student_list) {
        system("cls");
    printTitle("Add Student");
    if (!waitlist || !waitlist->front) {
        printf("Waitlist is empty. No students to add.\n");
        return;
    }

    char user_choice;
    do {
        // Display waitlist and ask if the user wants to view it
        printf(BRIGHT_YELLOW"Do you want to view the waitlist? (y/n): "RESET);
        scanf(" %c", &user_choice);

        if (user_choice == 'y' || user_choice == 'Y') {
            view_waitlist(waitlist);
        }

        // Ask for CIN to add a student from the waitlist
        char cin_to_add[9];
        do {
            printf(BRIGHT_YELLOW"Enter the CIN of the student you want to add to the class: "RESET);
            scanf("%s", cin_to_add);
        } while (!validateCIN(cin_to_add));

        // Find the student in the waitlist
        Waitlist* prev = NULL;
        Waitlist* current = waitlist->front;
        int found=0;
        while (current) {
            if (strcmp(current->data.cin, cin_to_add) == 0) {
                found=1;
                // Ask for the class to assign the student to
                char class_name[10];
                printf(BRIGHT_YELLOW"Enter the class to add the student to: "RESET);
                scanf("%s", class_name);

                // Assign the student to the class
                strncpy(current->data.class, class_name, sizeof(current->data.class));

                // Add student to the student list
                students* new_student = (students*)malloc(sizeof(students));
                new_student->student = current->data;
                new_student->next = *student_list;
                *student_list = new_student;

                // Remove student from the waitlist
                if (prev) {
                    prev->next = current->next;
                } else {
                    waitlist->front = current->next; // Update front if it's the first element
                }
                if (current == waitlist->rear) {
                    waitlist->rear = prev; // Update rear if it's the last element
                }

                free(current); // Free memory for the removed node
                current = NULL; // Reset pointer to avoid misuse

                // Save the updated student list to the file
                saveStudentsToFile(*student_list, "../data/students.txt");

                // Now, delete the student from the waitlist file
                updateWaitlistFile(waitlist);

                printf("Student %s %s (CIN: %s) has been added to class %s.\n",
                       new_student->student.first_name, new_student->student.last_name,
                       new_student->student.cin, class_name);
                break;
            }
            prev = current;
            current = current->next;
        }

        // If the student is not found
        if (!found) {
            printf("Student with CIN %s not found in the waitlist.\n", cin_to_add);
        }

        // Ask if the user wants to add another student
        printf(BRIGHT_YELLOW"Do you want to add another student? (y/n): "RESET);
        scanf(" %c", &user_choice);

    } while (user_choice == 'y' || user_choice == 'Y');
    printDivider();
}

void delete_student(students** student_list) {
    system("cls");
    printTitle("Delete Student");
    students *current = *student_list, *prev = NULL;

    // Ask for CIN to add a student from the waitlist
    char cin[9];
    do {
        printf(BRIGHT_YELLOW"Enter the CIN of the student you want to delete: "RESET);
        scanf("%s", cin);
    }while (!validateCIN(cin));

    while (current) {
        if (strcmp(current->student.cin, cin) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                *student_list = current->next;
            }
            free(current);
            printf(GREEN"Student with CIN %s deleted successfully.\n"RESET, cin);
            saveStudentsToFile(*student_list,"../data/students.txt");
            return;
        }
        prev = current;
        current = current->next;
    }

    printf(RED"Student with CIN %s not found.\n"RESET, cin);
    printDivider();
}

// Function to update student information
void update_student_info(students* student_list) {
    system("cls");
    printTitle("Update Student Info");
    students* current = student_list;
    char cin[9];
    do {
        printf(BRIGHT_YELLOW"Enter the CIN of the student you want to update: "RESET);
        scanf("%s", cin);
    }while (!validateCIN(cin));

    while (current) {
        if (strcmp(current->student.cin, cin) == 0) {
            printf("Student found: %s %s (CIN: %s)\n", current->student.first_name, current->student.last_name, current->student.cin);

            int choice;
            do {
                system("cls");
                printf(BRIGHT_YELLOW"\nWhat information would you like to update?\n"RESET);
                printMenuOption(1,"First Name",1);
                printMenuOption(2,"Last Name",1);
                printMenuOption(3,"Class",1);
                printMenuOption(4,"Email",1);
                printMenuOption(5,"Phone Number",1);
                printMenuOption(6,"Address",1);
                printMenuOption(7,"City",1);
                printMenuOption(8,"Username",1);
                printMenuOption(9,"Password",1);
                printMenuOption(0,"Return",1);
                getChoice(&choice);

                switch (choice) {
                    case 1:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                        printf(BRIGHT_YELLOW"Enter new First Name: "RESET);
                        fgets(current->student.first_name, sizeof(current->student.first_name), stdin);
                        current->student.first_name[strcspn(current->student.first_name, "\n")] = '\0';  // Remove trailing newline
                    simulateProgressBar(2);
                    printSuccess("First Name updated successfully.");
                    break;
                    case 2:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                        printf(BRIGHT_YELLOW"Enter new Last Name: "RESET);
                        fgets(current->student.last_name, sizeof(current->student.last_name), stdin);
                        current->student.last_name[strcspn(current->student.last_name, "\n")] = '\0';
                    simulateProgressBar(2);
                    printSuccess("Last Name updated successfully.");
                    break;
                    case 3:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                        printf(BRIGHT_YELLOW"Enter new Class: "RESET);
                    fgets(current->student.class, sizeof(current->student.class), stdin);
                    current->student.class[strcspn(current->student.class, "\n")] = '\0';
                    simulateProgressBar(2);
                    printSuccess("Class updated successfully.");
                    break;
                    case 4:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                        do {
                            printf(BRIGHT_YELLOW"Enter new email: "RESET);
                            fgets(current->student.email, sizeof(current->student.email), stdin);
                        }while (!validateEmail(current->student.email));
                        current->student.email[strcspn(current->student.email, "\n")] = '\0';
                        simulateProgressBar(2);
                        printSuccess("Email updated successfully.");
                        break;
                    case 5:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                        printf(BRIGHT_YELLOW"Enter new phone number: "RESET);
                        fgets(current->student.phone_number, sizeof(current->student.phone_number), stdin);
                        current->student.phone_number[strcspn(current->student.phone_number, "\n")] = '\0';
                    simulateProgressBar(2);
                    printSuccess("Phone number updated successfully.");
                    break;
                    case 6:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                        printf(BRIGHT_YELLOW"Enter new address: "RESET);
                        fgets(current->student.address, sizeof(current->student.address), stdin);
                        current->student.address[strcspn(current->student.address, "\n")] = '\0';
                    simulateProgressBar(2);
                    printSuccess("Address updated successfully.");
                    break;
                    case 7:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                        printf(BRIGHT_YELLOW"Enter new City: "RESET);
                        fgets(current->student.city, sizeof(current->student.city), stdin);
                        current->student.city[strcspn(current->student.city, "\n")] = '\0';
                    simulateProgressBar(2);
                    printSuccess("City updated successfully.");
                    break;
                    case 8:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                        printf(BRIGHT_YELLOW"Enter new username: "RESET);
                        fgets(current->student.username, sizeof(current->student.username), stdin);
                        current->student.username[strcspn(current->student.username, "\n")] = '\0';
                    simulateProgressBar(2);
                    printSuccess("Username updated successfully.");
                    break;
                    case 9:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                        do {
                            printf(BRIGHT_YELLOW"Enter new password: "RESET);
                            fgets(current->student.password, sizeof(current->student.password), stdin);
                        }while (!validatePassword(current->student.password));
                        current->student.password[strcspn(current->student.password, "\n")] = '\0';
                    simulateProgressBar(2);
                    printSuccess("Password updated successfully.");
                    break;
                    case 0:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                        printf("Canceling update...\n");
                        return;
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
                    break;
                }

            } while (1);

            // After updating, save the student list back to file
            saveStudentsToFile(student_list, "../data/students.txt");

            printf("Student information updated successfully for CIN %s.\n", cin);
            return;
        }
        current = current->next;
    }

    printf("Student with CIN %s not found.\n", cin);
    printDivider();
}

void manage_students(const char *role,students* students) {
    int choice;
    int access;
    if (strcmp(role, "Principal") == 0 || strcmp(role, "Registrar") == 0) {
        access = 1;
    }else access = 0;

    do {
        system("cls");
        printTitle("Manage Students");
        printMenuOption(1,"Add Student from waitlist",access);
        printMenuOption(2,"Delete Student",access);
        printMenuOption(3,"Update Student Info",access);
        printMenuOption(4,"View Students",1);
        printMenuOption(0,"Back to Manage Users",1);
        getChoice(&choice);

        if (choice >= 1 && choice <= 3 && strcmp(role, "Principal") != 0 && strcmp(role, "Registrar") != 0) {
            printf(RED"Access Denied: You do not have permission to perform this action.\n"RESET);
            stopMP3();
            playMP3("../sound/access-denied-102628.mp3");
            sleep(2);

            waitForUser();
            continue;
        }else {
            stopMP3();
            playMP3("../sound/access-granted-87075.mp3");
            sleep(2);
        }

        switch (choice) {
            case 1:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                WaitlistQueue waitlist;
                initializeQueue(&waitlist);
                loadWaitlistFromFile(&waitlist,"../data/registration.txt");
                addStudent(&waitlist,&students);
            waitForUser();
            break;
            case 2:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                delete_student(&students);
            waitForUser();
            break;
            case 3:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                update_student_info(students);
            waitForUser();
            break;
            case 4:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                show_students_info(students);
            waitForUser();
            break;
            case 0:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                printf("Returning to Manage Users...\n");
            break;
            default:
                stopMP3();
            playMP3("../sound/wrong-47985.mp3");
            printError("Invalid choice. Please try again.");
            break;
        }
    } while (choice != 0);
}

// Function to prompt user and add a new teacher
void add_teacher_prompt(teachers **head) {
    system("cls");
    printTitle("Add Teacher");
    printf("-----------------------------------------\n");
    teacher new_teacher;

    // Asking for teacher information
    printTitle("Enter Teacher Information");
    int valid = 0;
    do {
        printf(BRIGHT_YELLOW "Enter CIN: "RESET);
        scanf("%s", new_teacher.cin);
        valid = validateCIN(new_teacher.cin) && teacherExistsByCIN(*head,new_teacher.cin);
        getchar();  // To consume the newline character left by scanf
    }while (valid == 0);

    printf(BRIGHT_YELLOW"Enter First Name: "RESET);
    fgets(new_teacher.first_name, sizeof(new_teacher.first_name), stdin);
    new_teacher.first_name[strcspn(new_teacher.first_name, "\n")] = '\0';  // Remove trailing newline

    printf(BRIGHT_YELLOW"Enter Last Name: "RESET);
    fgets(new_teacher.last_name, sizeof(new_teacher.last_name), stdin);
    new_teacher.last_name[strcspn(new_teacher.last_name, "\n")] = '\0';

    do {
        printf(BRIGHT_YELLOW"Enter Date of Birth (YYYY-MM-DD): "RESET);
    scanf("%d-%d-%d", &new_teacher.date_of_birth.year, &new_teacher.date_of_birth.month, &new_teacher.date_of_birth.day);
        if (!isValidDate(new_teacher.date_of_birth)) {
            printf(RED"Invalid date. Please try again.\n"RESET);
        }
    }while (!isValidDate(new_teacher.date_of_birth));
    printf(BRIGHT_YELLOW"Enter Gender (M/F): "RESET);
    scanf(" %c", &new_teacher.gender);

    do {
        fflush(stdin);
        printf(BRIGHT_YELLOW"Enter Email: "RESET);
        fgets(new_teacher.email, sizeof(new_teacher.email), stdin);
        new_teacher.email[strcspn(new_teacher.email, "\n")] = '\0';
    }while (!validateEmail(new_teacher.email));

    printf(BRIGHT_YELLOW"Enter Phone Number: "RESET);
    fgets(new_teacher.phone_number, sizeof(new_teacher.phone_number), stdin);
    new_teacher.phone_number[strcspn(new_teacher.phone_number, "\n")] = '\0';

    printf(BRIGHT_YELLOW"Enter Address: "RESET);
    fgets(new_teacher.address, sizeof(new_teacher.address), stdin);
    new_teacher.address[strcspn(new_teacher.address, "\n")] = '\0';

    printf(BRIGHT_YELLOW"Enter City: "RESET);
    fgets(new_teacher.city, sizeof(new_teacher.city), stdin);
    new_teacher.city[strcspn(new_teacher.city, "\n")] = '\0';

    printf(BRIGHT_YELLOW"Enter Department: "RESET);
    fgets(new_teacher.departement, sizeof(new_teacher.departement), stdin);
    new_teacher.departement[strcspn(new_teacher.departement, "\n")] = '\0';

    printf(BRIGHT_YELLOW"Enter Subject: "RESET);
    fgets(new_teacher.subject, sizeof(new_teacher.subject), stdin);
    new_teacher.subject[strcspn(new_teacher.subject, "\n")] = '\0';

    printf(BRIGHT_YELLOW"Enter Username: "RESET);
    fgets(new_teacher.username, sizeof(new_teacher.username), stdin);
    new_teacher.username[strcspn(new_teacher.username, "\n")] = '\0';

    do {
        printf(BRIGHT_YELLOW"Enter Password: "RESET);
    fgets(new_teacher.password, sizeof(new_teacher.password), stdin);
    new_teacher.password[strcspn(new_teacher.password, "\n")] = '\0';
    }while (!validatePassword(new_teacher.password));

    // Creating and adding the new teacher to the list
    teachers* new_teacher_node = create_teacher(new_teacher);
    add_teacher(head, new_teacher_node);
    saveTeachersToFile(*head,"../data/teachers.txt");
    printDivider();
}

// Function to delete a teacher by CIN
void delete_teacher(teachers** teacher_list) {
    system("cls");
    printTitle("Delete Teacher");
    teachers *current = *teacher_list, *prev = NULL;

    // Ask for CIN to add a student from the waitlist
    char cin[9];
    do {
        printf(BRIGHT_YELLOW"Enter the CIN of the teacher you want to delete: "RESET);
        scanf("%s", cin);
    }while (!validateCIN(cin));
    while (current) {
        if (strcmp(current->teacher.cin, cin) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                *teacher_list = current->next;
            }
            free(current);
            printf("Teacher with CIN %s deleted successfully.\n", cin);
            saveTeachersToFile(*teacher_list,"../data/teachers.txt");
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Teacher with CIN %s not found.\n", cin);
    printDivider();

}

// Function to update teacher information
void update_teacher_info(teachers* teacher_list) {
    system("cls");
    printTitle("Update Teacher Info");
    teachers* current = teacher_list;
    char cin[9];
    do {
        printf(BRIGHT_YELLOW"Enter the CIN of the teacher you want to update: "RESET);
        scanf("%s", cin);
    }while (!validateCIN(cin));
    while (current) {
        if (strcmp(current->teacher.cin, cin) == 0) {
            printf("Teacher found: %s %s (CIN: %s)\n", current->teacher.first_name, current->teacher.last_name, current->teacher.cin);

            int choice;
            do {
                system("cls");
                printf(BRIGHT_YELLOW"\nWhat information would you like to update?\n"RESET);
                printMenuOption(1,"First Name",1);
                printMenuOption(2,"Last Name",1);
                printMenuOption(3,"Email",1);
                printMenuOption(4,"Phone Number",1);
                printMenuOption(5,"Address",1);
                printMenuOption(6,"City",1);
                printMenuOption(7,"Department",1);
                printMenuOption(8,"Subject",1);
                printMenuOption(9,"Username",1);
                printMenuOption(10,"Password",1);
                printMenuOption(0,"Return",1);

                getChoice(&choice);

                switch (choice) {
                    case 1:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                    printf(BRIGHT_YELLOW"Enter new First Name: "RESET);
                        fgets(current->teacher.first_name, sizeof(current->teacher.first_name), stdin);
                        current->teacher.first_name[strcspn(current->teacher.first_name, "\n")] = '\0';  // Remove trailing newline
                    simulateProgressBar(2);
                    printSuccess("First Name updated successfully.");
                    break;
                    case 2:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                    printf(BRIGHT_YELLOW"Enter new Last Name: "RESET);
                        fgets(current->teacher.last_name, sizeof(current->teacher.last_name), stdin);
                        current->teacher.last_name[strcspn(current->teacher.last_name, "\n")] = '\0';
                    simulateProgressBar(2);
                    printSuccess("Last Name updated successfully.");
                    break;
                    case 3:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                    do {
                        printf(BRIGHT_YELLOW"Enter new email: "RESET);
                        fgets(current->teacher.email, sizeof(current->teacher.email), stdin);
                        current->teacher.email[strcspn(current->teacher.email, "\n")] = '\0';
                    }while (!validateEmail(current->teacher.email));

                    simulateProgressBar(2);
                    printSuccess("Email updated successfully.");
                    break;
                    case 4:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                    printf(BRIGHT_YELLOW"Enter new phone number: "RESET);
                        fgets(current->teacher.phone_number, sizeof(current->teacher.phone_number), stdin);
                        current->teacher.phone_number[strcspn(current->teacher.phone_number, "\n")] = '\0';
                    simulateProgressBar(2);
                    printSuccess("Phone number updated successfully.");
                    break;
                    case 5:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                    printf(BRIGHT_YELLOW"Enter new address: "RESET);
                        fgets(current->teacher.address, sizeof(current->teacher.address), stdin);
                        current->teacher.address[strcspn(current->teacher.address, "\n")] = '\0';
                    simulateProgressBar(2);
                    printSuccess("Address updated successfully.");
                    break;
                    case 6:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                    printf(BRIGHT_YELLOW"Enter new City: "RESET);
                        fgets(current->teacher.city, sizeof(current->teacher.city), stdin);
                        current->teacher.city[strcspn(current->teacher.city, "\n")] = '\0';
                    simulateProgressBar(2);
                    printSuccess("City updated successfully.");
                    break;
                    case 7:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                        printf(BRIGHT_YELLOW"Enter new Department: "RESET);
                        fgets(current->teacher.departement, sizeof(current->teacher.departement), stdin);
                        current->teacher.departement[strcspn(current->teacher.departement, "\n")] = '\0';
                    simulateProgressBar(2);
                    printSuccess("Department updates successfully");
                    break;
                    case 8:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                        printf("Enter new Subject: ");
                        fgets(current->teacher.subject, sizeof(current->teacher.subject), stdin);
                        current->teacher.subject[strcspn(current->teacher.subject, "\n")] = '\0';
                    simulateProgressBar(2);
                    printSuccess("Subject updates successfully");
                    break;
                    case 9:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                    printf(BRIGHT_YELLOW"Enter new username: "RESET);
                        fgets(current->teacher.username, sizeof(current->teacher.username), stdin);
                        current->teacher.username[strcspn(current->teacher.username, "\n")] = '\0';
                    simulateProgressBar(2);
                    printSuccess("Username updated successfully.");
                    break;
                    case 10:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                    do {
                        printf(BRIGHT_YELLOW"Enter new password: "RESET);
                        fgets(current->teacher.password, sizeof(current->teacher.password), stdin);
                        current->teacher.password[strcspn(current->teacher.password, "\n")] = '\0';
                    }while (!validatePassword(current->teacher.password));

                    simulateProgressBar(2);
                    printSuccess("Password updated successfully.");
                    break;
                    case 0:
                        stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                    printf("Canceling update...\n");
                    return;
                    default:
                        stopMP3();
                    playMP3("../sound/wrong-47985.mp3");
                    printError("Invalid choice. Please try again.");
                    break;
                }

                // Confirm the update
                printf("Do you want to update another field? (y/n): ");
                char cont;
                scanf(" %c", &cont);
                if (cont != 'y' && cont != 'Y') {
                    break;
                }

            } while (1);

            // After updating, save the teacher list back to file
            saveTeachersToFile(teacher_list, "../data/teachers.txt");

            printf("Teacher information updated successfully for CIN %s.\n", cin);
            return;
        }
        current = current->next;
    }

    printf("Teacher with CIN %s not found.\n", cin);
}

// Function to print the table header for teachers
void TeacherTableHeader() {
    printf(BRIGHT_BLACK"-------------------------------------------------------------"
           "-----------------------------------------------------------------------\n"RESET);
    printf("|  %sCIN   %s|%sFirst Name%s| %sLast Name %s|%sBirth Date%s|%sGender%s|"
           "        %sEmail        %s|%sPhone Number%s|        %sAddress       %s|    %sCity    %s|%sDept.%s|%sSubject%s|\n",
           CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET);
    printf(BRIGHT_BLACK"-------------------------------------------------------------"
                       "---------------------------------------------------------------------------\n"RESET);
}

// Function to print a single teacher's row in the table
void TeacherRow(const teacher *t) {
    printf("|%-8s|%-10s|%-11s|%04d-%02d-%02d|%-6c|%-21s|%-12s|%-22s|%-12s|%-6s|%-8s|\n",
           t->cin, t->first_name, t->last_name,
           t->date_of_birth.year, t->date_of_birth.month, t->date_of_birth.day,
           t->gender, t->email, t->phone_number,
           t->address, t->city, t->departement, t->subject);
    printf(BRIGHT_BLACK"------------------------------------------------------------------------"
                       "-----------------------------------------------------------------------\n"RESET);
}

// Function to show the list of teachers
void show_teachers_info(teachers* teachers) {
    system("cls");
    printTitle("The list of teachers");

    // Check if the list is empty
    if (teachers == NULL) {
        printf("No teachers found.\n");
        return;
    }

    // Print the table header
    TeacherTableHeader();

    // Traverse the linked list and print each teacher's data
    while (teachers != NULL) {
        TeacherRow(&teachers->teacher);
        teachers = teachers->next;
    }
    printDivider();
}


void manage_teachers(const char *role,teachers *teachers) {
    int tchoice;
    system("cls");
    int access;
    if (strcmp(role, "Principal") == 0 || strcmp(role, "Registrar") == 0) {
        access = 1;
    }else access = 0;

    do {
        system("cls");
        printTitle("Manage Teachers");
        printMenuOption(1,"Add Teacher",access);
        printMenuOption(2,"Delete Teacher",access);
        printMenuOption(3,"Update Teacher Info",access);
        printMenuOption(4,"View Teachers",1);
        printMenuOption(0,"Back to Manage Users",1);
        getChoice(&tchoice);

        if (tchoice >= 1 && tchoice <= 3 && strcmp(role, "Principal") != 0) {
            printf("Access Denied: You do not have permission to perform this action.\n");
            stopMP3();
            playMP3("../sound/access-denied-102628.mp3");
            sleep(2);
            waitForUser();
            continue;
        }

        switch (tchoice) {
            case 1:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                add_teacher_prompt(&teachers);
            waitForUser();
            break;
            case 2:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                delete_teacher(&teachers);
            waitForUser();
            break;
            case 3:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                update_teacher_info(teachers);
            waitForUser();
            break;
            case 4:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
                show_teachers_info(teachers);
            waitForUser();
            break;
            case 0:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
            printf("Returning to Manage Users...\n");
            break;
            default:
                stopMP3();
            playMP3("../sound/wrong-47985.mp3");
            printError("Invalid choice. Please try again.");
            break;

        }
    } while (tchoice != 0);
}

// Function to print the table header for admins
void AdminTableHeader() {
    printf(BRIGHT_BLACK"-------------------------------------------------------------"
           "---------------------------------------------------------------------------------------\n"RESET);
    printf("|  %sCIN   %s|%sFirst Name%s| %sLast Name %s|%sBirth Date%s|%sGender%s|"
           "        %sEmail        %s|%sPhone Number%s|        %sAddress       %s|    %sCity    %s|%sRole  %s|%sUsername%s|\n",
           CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET,CYAN,RESET);
    printf(BRIGHT_BLACK"-------------------------------------------------------------"
                       "-----------------------------------------------------------------------------------------\n"RESET);
}

// Function to print a single admin's row in the table
void AdminRow(const admin *a) {
    printf("|%-8s|%-10s|%-11s|%04d-%02d-%02d|%-6c|%-21s|%-12s|%-22s|%-12s|%-7s|%-10s|\n",
           a->cin, a->first_name, a->last_name,
           a->date_of_birth.year, a->date_of_birth.month, a->date_of_birth.day,
           a->gender, a->email, a->phone_number,
           a->address, a->city, a->role, a->username);
    printf(BRIGHT_BLACK"------------------------------------------------------------------------"
                       "-----------------------------------------------------------------------------\n"RESET);
}

// Function to show the list of admins
void show_admins_info(admins* admins) {
    system("cls");
    printTitle("The list of admins");

    // Check if the list is empty
    if (admins == NULL) {
        printf("No admins found.\n");
        return;
    }

    // Print the table header
    AdminTableHeader();

    // Traverse the linked list and print each admin's data
    while (admins != NULL) {
        AdminRow(&admins->admin);
        admins = admins->next;
    }
    printDivider();
}

// Function to prompt user and add a new admin
void add_admin_prompt(admins **head) {
    system("cls");
    printTitle("Add Admin");
    admin new_admin;
    printf("---------------------------------------------\n");
    // Asking for admin information
    printTitle("Enter Admin Information");
    int valid = 0;
    do {
        printf(BRIGHT_YELLOW "Enter CIN: "RESET);
        scanf("%s", new_admin.cin);
        valid = validateCIN(new_admin.cin) && adminExistsByCIN(*head, new_admin.cin);
        getchar();  // Consume the newline character left by scanf
    } while (valid == 0);

    printf(BRIGHT_YELLOW"Enter First Name: "RESET);
    fgets(new_admin.first_name, sizeof(new_admin.first_name), stdin);
    new_admin.first_name[strcspn(new_admin.first_name, "\n")] = '\0';  // Remove trailing newline

    printf(BRIGHT_YELLOW"Enter Last Name: "RESET);
    fgets(new_admin.last_name, sizeof(new_admin.last_name), stdin);
    new_admin.last_name[strcspn(new_admin.last_name, "\n")] = '\0';

    do {
        printf(BRIGHT_YELLOW"Enter Date of Birth (YYYY-MM-DD): "RESET);
        scanf("%d-%d-%d", &new_admin.date_of_birth.year, &new_admin.date_of_birth.month, &new_admin.date_of_birth.day);
        if (!isValidDate(new_admin.date_of_birth)) {
            printf(RED"Invalid date. Please try again.\n"RESET);
        }
    } while (!isValidDate(new_admin.date_of_birth));

    printf(BRIGHT_YELLOW"Enter Gender (M/F): "RESET);
    scanf(" %c", &new_admin.gender);

    do {
        fflush(stdin);
        printf(BRIGHT_YELLOW"Enter Email: "RESET);
        fgets(new_admin.email, sizeof(new_admin.email), stdin);
        new_admin.email[strcspn(new_admin.email, "\n")] = '\0';
    } while (!validateEmail(new_admin.email));

    printf(BRIGHT_YELLOW"Enter Phone Number: "RESET);
    fgets(new_admin.phone_number, sizeof(new_admin.phone_number), stdin);
    new_admin.phone_number[strcspn(new_admin.phone_number, "\n")] = '\0';

    printf(BRIGHT_YELLOW"Enter Address: "RESET);
    fgets(new_admin.address, sizeof(new_admin.address), stdin);
    new_admin.address[strcspn(new_admin.address, "\n")] = '\0';

    printf(BRIGHT_YELLOW"Enter City: "RESET);
    fgets(new_admin.city, sizeof(new_admin.city), stdin);
    new_admin.city[strcspn(new_admin.city, "\n")] = '\0';

    printf(BRIGHT_YELLOW"Enter Role: "RESET);
    fgets(new_admin.role, sizeof(new_admin.role), stdin);
    new_admin.role[strcspn(new_admin.role, "\n")] = '\0';

    printf(BRIGHT_YELLOW"Enter Username: "RESET);
    fgets(new_admin.username, sizeof(new_admin.username), stdin);
    new_admin.username[strcspn(new_admin.username, "\n")] = '\0';

    do {
        printf(BRIGHT_YELLOW"Enter Password: "RESET);
        fgets(new_admin.password, sizeof(new_admin.password), stdin);
        new_admin.password[strcspn(new_admin.password, "\n")] = '\0';
    } while (!validatePassword(new_admin.password));

    // Creating and adding the new admin to the list
    admins* new_admin_node = create_admin(new_admin);
    add_admin(head, new_admin_node);
    saveAdminsToFile(*head, "../data/admins.txt");
    printDivider();
}

// Function to delete an admin by CIN
void delete_admin(admins** admin_list) {
    system("cls");
    printTitle("Delete Admin");
    admins* current = *admin_list;
    admins* prev = NULL;
    char cin[9];

    // Prompt for CIN
    do {
        printf(BRIGHT_YELLOW"Enter the CIN of the admin you want to delete: "RESET);
        scanf("%8s", cin);
    } while (!validateCIN(cin));

    // Traverse the list to find the admin
    while (current) {
        if (strcmp(current->admin.cin, cin) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                *admin_list = current->next; // Update head if first node is deleted
            }
            free(current);
            printf(GREEN "Admin with CIN %s deleted successfully.\n" RESET, cin);
            saveAdminsToFile(*admin_list, "../data/admins.txt"); // Save updated list to file
            return;
        }
        prev = current;
        current = current->next;
    }

    // If CIN is not found
    printf(RED "Admin with CIN %s not found.\n" RESET, cin);
    printDivider();
}

// Function to update admin information
void update_admin_info(admins* admin_list) {
    system("cls");
    printTitle("Update Admin Info");
    admins* current = admin_list;
    char cin[9];

    // Prompt for CIN
    do {
        printf(BRIGHT_YELLOW"Enter the CIN of the admin you want to update: "RESET);
        scanf("%8s", cin);
    } while (!validateCIN(cin));

    // Traverse the list to find the admin
    while (current) {
        if (strcmp(current->admin.cin, cin) == 0) {
            printf(GREEN "Admin found: %s %s (CIN: %s)\n" RESET, current->admin.first_name, current->admin.last_name, current->admin.cin);

            int choice;
            do {
                system("cls");
                printf(BRIGHT_YELLOW"\nWhat information would you like to update?\n"RESET);
                printMenuOption(1, "First Name", 1);
                printMenuOption(2, "Last Name", 1);
                printMenuOption(3, "Email", 1);
                printMenuOption(4, "Phone Number", 1);
                printMenuOption(5, "Address", 1);
                printMenuOption(6, "City", 1);
                printMenuOption(7, "Role", 1);
                printMenuOption(8, "Username", 1);
                printMenuOption(9, "Password", 1);
                printMenuOption(0, "Return", 1);

                getChoice(&choice);

                switch (choice) {
                    case 1:
                        printf(BRIGHT_YELLOW"Enter new First Name: "RESET);
                        fgets(current->admin.first_name, sizeof(current->admin.first_name), stdin);
                        current->admin.first_name[strcspn(current->admin.first_name, "\n")] = '\0';
                        printSuccess("First Name updated successfully.");
                        break;
                    case 2:
                        printf(BRIGHT_YELLOW"Enter new Last Name: "RESET);
                        fgets(current->admin.last_name, sizeof(current->admin.last_name), stdin);
                        current->admin.last_name[strcspn(current->admin.last_name, "\n")] = '\0';
                        printSuccess("Last Name updated successfully.");
                        break;
                    case 3:
                        do {
                            printf(BRIGHT_YELLOW"Enter new Email: "RESET);
                            fgets(current->admin.email, sizeof(current->admin.email), stdin);
                            current->admin.email[strcspn(current->admin.email, "\n")] = '\0';
                        } while (!validateEmail(current->admin.email));
                        printSuccess("Email updated successfully.");
                        break;
                    case 4:
                        printf(BRIGHT_YELLOW"Enter new Phone Number: "RESET);
                        fgets(current->admin.phone_number, sizeof(current->admin.phone_number), stdin);
                        current->admin.phone_number[strcspn(current->admin.phone_number, "\n")] = '\0';
                        printSuccess("Phone Number updated successfully.");
                        break;
                    case 5:
                        printf(BRIGHT_YELLOW"Enter new Address: "RESET);
                        fgets(current->admin.address, sizeof(current->admin.address), stdin);
                        current->admin.address[strcspn(current->admin.address, "\n")] = '\0';
                        printSuccess("Address updated successfully.");
                        break;
                    case 6:
                        printf(BRIGHT_YELLOW"Enter new City: "RESET);
                        fgets(current->admin.city, sizeof(current->admin.city), stdin);
                        current->admin.city[strcspn(current->admin.city, "\n")] = '\0';
                        printSuccess("City updated successfully.");
                        break;
                    case 7:
                        printf(BRIGHT_YELLOW"Enter new Role: "RESET);
                        fgets(current->admin.role, sizeof(current->admin.role), stdin);
                        current->admin.role[strcspn(current->admin.role, "\n")] = '\0';
                        printSuccess("Role updated successfully.");
                        break;
                    case 8:
                        printf(BRIGHT_YELLOW"Enter new Username: "RESET);
                        fgets(current->admin.username, sizeof(current->admin.username), stdin);
                        current->admin.username[strcspn(current->admin.username, "\n")] = '\0';
                        printSuccess("Username updated successfully.");
                        break;
                    case 9:
                        do {
                            printf(BRIGHT_YELLOW"Enter new Password: "RESET);
                            fgets(current->admin.password, sizeof(current->admin.password), stdin);
                            current->admin.password[strcspn(current->admin.password, "\n")] = '\0';
                        } while (!validatePassword(current->admin.password));
                        printSuccess("Password updated successfully.");
                        break;
                    case 0:
                        printf(GREEN "Returning to Admin Menu...\n" RESET);
                        return;
                    default:
                        printError("Invalid choice. Please try again.");
                        break;
                }
            } while (1);

            // Save updated list to file
            saveAdminsToFile(admin_list, "../data/admins.txt");

            printf(GREEN "Admin information updated successfully for CIN %s.\n" RESET, cin);
            return;
        }
        current = current->next;
    }

    // If CIN is not found
    printf(RED "Admin with CIN %s not found.\n" RESET, cin);
    printDivider();
}


void manage_admins(const char *role,admins* admins) {
    if (strcmp(role, "Principal") != 0) {
        printf("Access Denied: Only the Principal can manage admins.\n");
        stopMP3();
        playMP3("../sound/access-denied-102628.mp3");
        sleep(2);
        return;
    }
    int achoice;


    do {
        system("cls");
        printTitle("Manage Admins");
        printMenuOption(1, "Add Admin", 1);
        printMenuOption(2, "Delete Admin", 1);
        printMenuOption(3, "Update Admin Info", 1);
        printMenuOption(4, "View Admins", 1);  // All roles can view admins
        printMenuOption(0, "Back to Manage Users", 1);  // All roles can go back
        getChoice(&achoice);

        switch (achoice) {
            case 1:
                stopMP3();
                playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                sleep(1);
                add_admin_prompt(&admins);
                break;
            case 2:
                stopMP3();
                playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                sleep(1);
                delete_admin(&admins);
                break;
            case 3:
                stopMP3();
                playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                sleep(1);
                update_admin_info(admins);
                break;
            case 4:
                stopMP3();
                playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                sleep(1);
                show_admins_info(admins);
                waitForUser();  // This function can wait for user input to return to the menu
                break;
            case 0:
                stopMP3();
                playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                sleep(1);
                printf("Returning to Manage Users...\n");
                break;
            default:
                stopMP3();
                playMP3("../sound/wrong-47985.mp3");
                printError("Invalid choice. Please try again.");
                break;
        }
    } while (achoice != 0);
}


void manage_users(const char *role, students *students, teachers *teachers, admins *admins) {
    int choice;

    do {
        system("cls");
        printf(MOVE_CURSOR(0, 0));  // Move the cursor to top-left
        printTitle("Manage Users");

        // Display the menu options with role-based access control
        printMenuOption(1, "Manage Students", 1);
        printMenuOption(2, "Manage Teachers", 1);
        printMenuOption(3, "Manage Admins", strcmp(role, "Principal") == 0 ? 1 : 0);  // Only Principal can manage admins
        printMenuOption(0, "Back to Admin Menu", 1);

        // Get the user's choice
        getChoice(&choice);

        // Access control and menu handling
        switch (choice) {
            case 1:
                stopMP3();
                playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                sleep(1);
                manage_students(role, students);
                break;
            case 2:
                stopMP3();
                playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                sleep(1);
                manage_teachers(role, teachers);
                break;
            case 3:
                if (strcmp(role, "Principal") == 0) {
                    stopMP3();
                    playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                    sleep(1);
                    manage_admins(role, admins);
                } else {
                    stopMP3();
                    playMP3("../sound/access-denied-102628.mp3");
                    sleep(2);
                    printError("Access Denied: You do not have permission to manage admins.\n");
                }
                break;
            case 0:
                stopMP3();
                playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
                sleep(1);
                printf("Returning to Admin Menu...\n");
                break;
            default:
                stopMP3();
                playMP3("../sound/wrong-47985.mp3");
                printError("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 0);
}


void view_group_schedule() {
    char group_name[50]; // Define group name as a character array
    printTitle("Group Schedules");
    printf(BRIGHT_YELLOW "Enter Group: " RESET);
    scanf("%49s", group_name); // Ensure input does not exceed array size
    Group groups[MAX_GROUPS];
    int groupCount = 0;
    int found = 0;

    // Initialize groups array to avoid uninitialized data
    memset(groups, 0, sizeof(groups));

    // Load schedules into groups
    int scheduleCount = loadSchedules(groups, &groupCount);
    printf(BRIGHT_YELLOW "\nSchedule for Group %s:\n" RESET, group_name);

    if (scheduleCount > 0) {
        for (int i = 0; i < groupCount; i++) {
            // Check if the group matches the specified group name
            if (strcmp(groups[i].groupName, group_name) == 0) {
                for (int j = 0; j < DAYS_OF_WEEK; j++) {
                    const char* dayNames[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
                    printf(CYAN "\n%s:\n" RESET, dayNames[j]);
                    for (int k = 0; k < groups[i].courseCount[j]; k++) {
                        // Print course details for the day
                        printf("  %s - %s - Time: %s, Classroom: %s\n",
                               groups[i].courses[j][k].courseName, groups[i].courses[j][k].teacherName,
                               groups[i].courses[j][k].time, groups[i].courses[j][k].classroom);
                        found = 1;
                    }
                }
                break; // Break once the group is found and processed
            }
        }
    }

    // Handle case where no schedule is found
    if (!found) {
        printf(RED "No schedules loaded for Group %s.\n" RESET, group_name);
    }

    printDivider();
}

void saveTeacherScheduleToFile(const char* teacher_cin, TeacherScheduleNode* head) {
    FILE* file = fopen("../data/TeacherSchedules.txt", "r");
    if (!file) {
        printf(RED "Error opening file for reading.\n" RESET);
        return;
    }

    char buffer[10000] = {0};
    char line[512];
    int updated = 0;

    // Variables to temporarily hold parsed data from the file
    char tempCIN[9], tempSubject[50], tempDay[20], tempTime[20], tempClassroom[50], tempGroup[10];

    // Read existing file contents
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%8[^,], %49[^,], %19[^,], %19[^,], %49[^,], %9[^\n]",
                   tempCIN, tempSubject, tempDay, tempTime, tempClassroom, tempGroup) == 6) {

            int conflictFound = 0;
            TeacherScheduleNode* current = head;

            // Check for conflict in the file line
            while (current != NULL) {
                if (strcmp(tempCIN, teacher_cin) == 0 &&
                    strcmp(tempDay, current->schedule.day) == 0 &&
                    strcmp(tempTime, current->schedule.time) == 0) {
                    // Replace with the new schedule
                    snprintf(line, sizeof(line), "%s, %s, %s, %s, %s, %s\n",
                             teacher_cin, current->schedule.subject, current->schedule.day,
                             current->schedule.time, current->schedule.classroom, current->schedule.group);
                    conflictFound = 1;
                    updated = 1;
                    break;
                }
                current = current->next;
            }

            // Append the line (updated or original) to the buffer
            strcat(buffer, line);

            // Skip adding this line again if conflictFound
            if (conflictFound) {
                continue;
            }
        } else {
            // Append non-parsable lines (e.g., headers) as-is
            strcat(buffer, line);
        }
    }

    fclose(file);

    // Append remaining new schedules (those not already in the file)
    TeacherScheduleNode* current = head;
    while (current != NULL) {
        int exists = 0;

        // Check if this schedule already exists
        FILE* checkFile = fopen("../data/TeacherSchedules.txt", "r");
        if (checkFile) {
            while (fgets(line, sizeof(line), checkFile)) {
                sscanf(line, "%8[^,], %49[^,], %19[^,], %19[^,], %49[^,], %9[^\n]",
                       tempCIN, tempSubject, tempDay, tempTime, tempClassroom, tempGroup);

                if (strcmp(tempCIN, teacher_cin) == 0 &&
                    strcmp(tempDay, current->schedule.day) == 0 &&
                    strcmp(tempTime, current->schedule.time) == 0) {
                    exists = 1;
                    break;
                }
            }
            fclose(checkFile);
        }

        // Add the new schedule if it doesn't exist
        if (!exists) {
            snprintf(line, sizeof(line), "%s, %s, %s, %s, %s, %s\n",
                     teacher_cin, current->schedule.subject, current->schedule.day,
                     current->schedule.time, current->schedule.classroom, current->schedule.group);
            strcat(buffer, line);
        }

        current = current->next;
    }

    // Write the updated buffer back to the file
    file = fopen("../data/TeacherSchedules.txt", "w");
    if (!file) {
        printf(RED "Error opening file for writing.\n" RESET);
        return;
    }

    fputs(buffer, file);
    fclose(file);

    printf(GREEN "Teacher schedule saved successfully for CIN %s.\n" RESET, teacher_cin);
}

// Function to view teacher's schedule
void view_teacher_schedule(teachers* teacher_list) {
    char teacher_cin[9];
    do {
        printf(BRIGHT_YELLOW "Enter Teacher CIN: " RESET);
        scanf("%8s", teacher_cin);  // Get teacher CIN
    }while (teacherExistsByCIN(teacher_list,teacher_cin));


    TeacherScheduleNode* head = NULL;
    // Load the teacher's schedule from the file
    loadTeacherSchedules("../data/TeacherSchedules.txt", &head);

    if (head == NULL) {
        printf(RED "No schedule found for teacher CIN: %s.\n" RESET, teacher_cin);
        return;
    }

    printTitle("Teacher's Schedule");
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

            // Check if the schedule day matches the current day and CIN matches
            if (strcmp(schedule->day, days[i]) == 0 && strcmp(schedule->cin, teacher_cin) == 0) {
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
            printf(BRIGHT_BLACK "  No classes scheduled for this day.\n" RESET);
        }
    }
    printDivider();
}

void generateTeacherSchedule(teachers* teachers) {
    char teacher_cin[9];
    printTitle("Generate Teacher Schedule");

    do {
        printf(BRIGHT_YELLOW "Enter Teacher CIN: " RESET);
        scanf("%8s", teacher_cin);
    } while (teacherExistsByCIN(teachers, teacher_cin));  // Validate teacher existence

    TeacherScheduleNode* head = NULL;
    loadTeacherSchedules("../data/TeacherSchedules.txt", &head);

    char more_classes = 'y';
    while (more_classes == 'y' || more_classes == 'Y') {
        TeacherSchedule new_schedule;

        printf(BRIGHT_YELLOW "Enter Subject: " RESET);
        scanf(" %[^\n]", new_schedule.subject);

        printf(BRIGHT_YELLOW "Enter Day (e.g., Monday): " RESET);
        scanf(" %[^\n]", new_schedule.day);

        printf(BRIGHT_YELLOW "Enter Time (e.g., 10:00 AM): " RESET);
        scanf(" %[^\n]", new_schedule.time);

        printf(BRIGHT_YELLOW "Enter Classroom: " RESET);
        scanf(" %[^\n]", new_schedule.classroom);

        printf(BRIGHT_YELLOW "Enter Group: " RESET);
        scanf(" %[^\n]", new_schedule.group);

        TeacherScheduleNode* new_node = (TeacherScheduleNode*)malloc(sizeof(TeacherScheduleNode));
        if (!new_node) {
            printf(RED "Memory allocation failed.\n" RESET);
            return;
        }
        new_node->schedule = new_schedule;
        new_node->next = head;
        head = new_node;

        printf(GREEN "Class added successfully.\n" RESET);

        printf(BRIGHT_YELLOW "Would you like to add another class? (y/n): " RESET);
        scanf(" %c", &more_classes);
    }

    saveTeacherScheduleToFile(teacher_cin, head);
}


void saveGroupScheduleToFile(const char* group_name, Schedule* head) {
    FILE* file = fopen("../data/schedules.txt", "r");
    if (!file) {
        printf(RED "Error opening file for reading.\n" RESET);
        return;
    }

    char buffer[10000] = {0};
    char line[512];
    int updated = 0;

    // Buffer to track which lines have been processed (avoids duplicates)
    Schedule* current = head;
    char tempGroup[10], tempDay[20], tempCourse[50], tempTeacher[50], tempTime[20], tempClassroom[50];
    // Read existing file contents
    while (fgets(line, sizeof(line), file)) {


        if (sscanf(line, "%9[^,], %19[^,], %49[^,], %49[^,], %19[^,], %49[^\n]",
                   tempGroup, tempDay, tempCourse, tempTeacher, tempTime, tempClassroom) == 6) {

            int conflictFound = 0;
            const char* days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
            current = head;

            // Check for conflict in the file line
            while (current != NULL) {
                if (strcmp(tempGroup, group_name) == 0 &&
                    strcmp(tempDay, days[current->day - 1]) == 0 &&
                    strcmp(tempTime, current->time) == 0) {
                    // Replace with the new schedule
                    snprintf(line, sizeof(line), "%s, %s, %s, %s, %s, %s\n",
                             group_name, days[current->day - 1], current->courseName,
                             current->teacherName, current->time, current->classroom);
                    conflictFound = 1;
                    updated = 1;
                    break;
                }
                current = current->next;
            }

            // Append the line (updated or original) to the buffer
            strcat(buffer, line);

            // Skip adding this line again if conflictFound
            if (conflictFound) {
                continue;
            }
        } else {
            // Append non-parsable lines (e.g., headers) as-is
            strcat(buffer, line);
        }
    }

    fclose(file);

    // Append remaining new schedules (those not in the file yet)
    current = head;
    while (current != NULL) {
        const char* days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
        int exists = 0;

        // Check if this schedule is already updated
        FILE* checkFile = fopen("../data/schedules.txt", "r");
        if (checkFile) {
            while (fgets(line, sizeof(line), checkFile)) {
                sscanf(line, "%9[^,], %19[^,], %49[^,], %49[^,], %19[^,], %49[^\n]",
                       tempGroup, tempDay, tempCourse, tempTeacher, tempTime, tempClassroom);

                if (strcmp(tempGroup, group_name) == 0 &&
                    strcmp(tempDay, days[current->day - 1]) == 0 &&
                    strcmp(tempTime, current->time) == 0) {
                    exists = 1;
                    break;
                }
            }
            fclose(checkFile);
        }

        if (!exists) {
            snprintf(line, sizeof(line), "%s, %s, %s, %s, %s, %s\n",
                     group_name, days[current->day - 1], current->courseName,
                     current->teacherName, current->time, current->classroom);
            strcat(buffer, line);
        }

        current = current->next;
    }

    // Write the updated buffer back to the file
    file = fopen("../data/schedules.txt", "w");
    if (!file) {
        printf(RED "Error opening file for writing.\n" RESET);
        return;
    }

    fputs(buffer, file);
    fclose(file);

    printf(GREEN "Group schedule saved successfully for Group %s.\n" RESET, group_name);
}


// Function to generate a group schedule
void generate_group_schedule() {
    char group_name[10];
    printTitle("Generate Group Schedule");
    printf(BRIGHT_YELLOW "Enter Group Name: " RESET);
    scanf("%9s", group_name);

    Schedule* head = NULL;
    Schedule* tail = NULL;

    char more_courses = 'y';
    while (more_courses == 'y' || more_courses == 'Y') {
        Schedule new_schedule;

        printf(BRIGHT_YELLOW "Enter Course Name: " RESET);
        scanf(" %[^\n]", new_schedule.courseName);

        printf(BRIGHT_YELLOW "Enter Teacher Name: " RESET);
        scanf(" %[^\n]", new_schedule.teacherName);

        printf(BRIGHT_YELLOW "Enter Day (1 for Monday, 2 for Tuesday, etc.): " RESET);
        scanf("%d", &new_schedule.day);

        printf(BRIGHT_YELLOW "Enter Time (e.g., 9:00 AM - 11:00 AM): " RESET);
        scanf(" %[^\n]", new_schedule.time);

        printf(BRIGHT_YELLOW "Enter Classroom: " RESET);
        scanf(" %[^\n]", new_schedule.classroom);

        // Create and append new schedule node
        Schedule* new_node = (Schedule*)malloc(sizeof(Schedule));
        new_node->next = NULL;
        new_node->day = new_schedule.day;
        strcpy(new_node->courseName, new_schedule.courseName);
        strcpy(new_node->teacherName, new_schedule.teacherName);
        strcpy(new_node->time, new_schedule.time);
        strcpy(new_node->classroom, new_schedule.classroom);

        if (head == NULL) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }

        printf(GREEN "Course '%s' added successfully.\n" RESET, new_schedule.courseName);
        printf(BRIGHT_YELLOW "Add another course? (y/n): " RESET);
        scanf(" %c", &more_courses);
    }

    saveGroupScheduleToFile(group_name, head);

    // Free allocated memory
    Schedule* current = head;
    while (current) {
        Schedule* temp = current;
        current = current->next;
        free(temp);
    }
}

// Function to manage schedules
void manage_schedules(const char *role,teachers* teachers) {
    int choice;
    do {
        system("cls");
        printf(MOVE_CURSOR(0, 0));  // Move the cursor to the top-left
        printTitle("Manage Schedules");

        // Displaying the menu options with access control
        printMenuOption(1, "View Group Schedule", 1);
        printMenuOption(2, "Generate Group Schedule", 1);
        printMenuOption(3, "View Teacher Schedule", 1);
        printMenuOption(4, "Generate Teacher Schedule", 1);
        printMenuOption(0, "Back to Admin Menu", 1);

        // Get user choice
        getChoice(&choice);

        switch (choice) {
            case 1:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");

            view_group_schedule();
            sleep(1);
            waitForUser();
            break;
            case 2:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");

            generate_group_schedule();
            sleep(1);
            waitForUser();
            break;
            case 3:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");

            view_teacher_schedule(teachers);
            sleep(1);
            waitForUser();
            break;
            case 4:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");

            generateTeacherSchedule(teachers);
            sleep(1);
            break;
            case 0:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            sleep(1);
            printf("Returning to Admin Menu...\n");
            break;
            default:
                stopMP3();
            playMP3("../sound/wrong-47985.mp3");
            printError("Invalid choice. Please try again.");
            break;
        }
    } while (choice != 0);
}




void admin_ui(admin admin_logged_in, admins *admins, teachers *teachers, students *students) {
    int choice;
    const char *role = admin_logged_in.role;

    do {
        system("cls");
        char title[256];
        snprintf(title,sizeof(title),"Admin Interface (%s)",role);
        printTitle(title);

        // Display menu options with appropriate sounds and styles
        printMenuOption(1, "Manage Users", 1);
        printMenuOption(2, "Manage Schedules", 1);
        printMenuOption(0, "Logout", 1);

        // Get and validate the user choice
        getChoice(&choice);

        switch (choice) {
            case 1:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            manage_users(role, students, teachers, admins);
            break;

            case 2:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            manage_schedules(role, teachers);
            break;

            case 0:
                stopMP3();
            playMP3("../sound/mixkit-correct-answer-tone-2870.wav");
            simulateProgressBar(1);
            printf(GREEN "Logging out...\n" RESET);
            saveAdminsToFile(admins, "../data/admins.txt");
            saveTeachersToFile(teachers, "../data/teachers.txt");
            saveStudentsToFile(students, "../data/students.txt");
            simulateLoading(2, "Saving data");
            printf(GREEN "Data saved successfully. Goodbye!\n" RESET);
            sleep(2);
            return;  // Exit the admin interface

            default:
                stopMP3();
            playMP3("../sound/wrong-47985.mp3");
            printError("Invalid choice. Please select a valid option.");
            break;
        }
    } while (1);  // Loop until the user chooses to log out
}
