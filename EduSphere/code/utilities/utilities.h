#ifndef UTILITIES_H
#define UTILITIES_H

#define MAX_ASSIGNMENTS 100
#define maxSubmissions 100
#define MAX_SCHEDULES 100
#define MAX_COURSES_PER_DAY 3
#define MAX_GROUPS 20
#define DAYS_OF_WEEK 5
#define MAX_GRADES 100
#define MAX_ATTENDANCE_RECORDS 1000
#define MAX_NOTIFICATIONS 1000

#define MAX_STUDENTS 100
#define MAX_SUBJECTS 10
#define MAX_NAME_LENGTH 100
#define MAX_CIN_LENGTH 20
#define MAX_DATE_LENGTH 20
#define MAX_STATUS_LENGTH 10
#define MAX_SUBJECT_LENGTH 50

typedef struct date {
    int year;
    int month;
    int day;
}date;

typedef struct Student{
    char cin[9];
    char first_name[30];
    char last_name[30];
    date date_of_birth;
    char gender;
    char email[50];
    char phone_number[50];
    char address[100];
    char city[30];
    char class[10];
    char username[100];
    char password[100];
}student;
typedef struct Students {
    student student;
    struct Students *next;
}students;
// Queue node structure for the waitlist
typedef struct WaitlistNode {
    student data;
    struct WaitlistNode* next;
} Waitlist;

// Queue structure to manage waitlist
typedef struct {
    Waitlist* front;
    Waitlist* rear;
} WaitlistQueue;

typedef struct Teacher{
    char cin[9];
    char first_name[30];
    char last_name[30];
    date date_of_birth;
    char gender;
    char email[50];
    char phone_number[50];
    char address[100];
    char city[30];
    char departement[50];
    char subject[50];
    char username[100];
    char password[100];
}teacher;
typedef struct Teachers {
    teacher teacher;
    struct Teachers *next;
}teachers;

typedef struct Admin{
    char cin[9];
    char first_name[30];
    char last_name[30];
    date date_of_birth;
    char gender;
    char email[50];
    char phone_number[50];
    char address[100];
    char city[30];
    char role[15];
    char username[100];
    char password[100];
}admin;
typedef struct Admins {
    admin admin;
    struct Admins *next;
}admins;

// Assignment structure with class, name, and due date
typedef struct {
    char className[50];
    char name[100];
    date dueDate;
} Assignment;

typedef struct Schedule {
    char courseName[50];
    char teacherName[50];
    int day;
    char time[20];
    char classroom[50];
    struct Schedule* next;
} Schedule;
typedef struct {
    char cin[9];        // Teacher's CIN
    char subject[50];   // Subject taught by the teacher
    char day[20];       // Day of the week (e.g., "Monday")
    char time[20];      // Time of the class (e.g., "10:00 AM")
    char classroom[50]; // Classroom location
    char group[10];     // Group (e.g., "1A", "1B")
} TeacherSchedule;
typedef struct TeacherScheduleNode {
    TeacherSchedule schedule;
    struct TeacherScheduleNode* next;
} TeacherScheduleNode;
typedef struct {
    char courseName[50];   // Course name (e.g., Mathematics)
    char teacherName[50];  // Teacher name (e.g., Mr. Smith)
    char time[20];         // Class time (e.g., 09:00-10:30)
    char classroom[50];    // Classroom (e.g., Room 101)
} Course;
// Define the structure for a group, including schedules for each day
typedef struct {
    char groupName[10];    // Group name (e.g., 1A, 1B)
    Course courses[DAYS_OF_WEEK][MAX_COURSES_PER_DAY]; // 5 days of study with multiple courses per day
    int courseCount[DAYS_OF_WEEK];  // Number of courses for each day
} Group;

typedef struct {
    char CIN[9];
    char subject[20];
    float grade;
}grade;
typedef struct {
    date date;
    char cin[9];
    char firstName[30];   // Student's first name
    char lastName[30];
    char subject[20];
    char status;
}attendance;
typedef struct {
    char cin[9];
    char subject[20];
    int absenceCount;
}absences;
typedef struct {
    char cin[9];
    char subject[20];
    char reason[100];
}eleminations;
typedef struct {
    char cin[9];
    char notification[500];
    date date;
}notifications;
typedef struct College {
    int id;
    char name[100];
    char country[50];
    struct College *next;
} College;
typedef struct {
    char cin[9];
    char firstName[30];
    char lastName[30];
    char groupName[10];
    char assignmentName[50];
    date submissionDate;
} Submission;

void removeExpiredAssignments();

int isValidDate(date d);

void printBox(int n,const char *labels[]);

void printTitleBox();
void animatedText(const char* header);
void printMenuOption(int optionNumber, const char* description, int isActive);
void printTitle(const char* title);
void printDivider();
void printSuccess(const char* message);
void printError(const char* message);

void simulateLoading(int duration,const char *message);
void simulateProgressBar(int duration);
int validateCIN(const char* cin);
int validatePassword(const char* password);
void getChoice(int *choice);

void printButtons(int n,const char *labels[]);

students* load_students_from_file(const char *filename);

teachers* load_teachers_from_file(const char *filename);
teachers* create_teacher(teacher t);
void add_teacher(teachers **head, teachers *new_teacher);
admins* create_admin(admin a);
void add_admin(admins **head, admins *new_admin);
admins* load_admins_from_file(const char *filename);
int studentExistsByCIN(students* head, const char* cin);
int adminExistsByCIN(admins* head, const char* cin);
int studentExistsInQueue(WaitlistQueue* queue, const char* cin);
int validateEmail(const char* email);
int isDatePassed(date d1, date d2);
int teacherExistsByCIN(teachers* head, const char* cin);
// Function to load assignments from a file
int loadAssignments(Assignment assignments[]);
int loadSchedules(Group groups[], int* groupCount);
int loadGrades(grade grades[]);
void saveAssignments(const char* filename,Assignment assignments[], int count);
int loadSubmissions(const char* filename, Submission submissions[]);
int loadTeacherSchedules(const char* filename, TeacherScheduleNode** head);
void saveGradesToFile(const char* filename, grade grades[], int gradeCount);
void getCurrentDate(char* dateBuffer);
void waitForUser();
void playMP3(const char *filePath);
void stopMP3();

#endif //UTILITIES_H
