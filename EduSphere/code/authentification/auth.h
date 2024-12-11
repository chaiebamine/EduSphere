//
// Created by Asus on 30/11/2024.
//

#ifndef AUTH_H
#define AUTH_H


void auth(const char *role);
void saveStudentsToFile(students* students_login,const char* filename);
void saveTeachersToFile(teachers *head, const char *filename);
void loadWaitlistFromFile(WaitlistQueue* queue, const char* filename);
void saveWaitlistToFile(WaitlistQueue* queue, const char* filename);
void initializeQueue(WaitlistQueue* queue);
void enqueue(WaitlistQueue* queue, student new_student);
void getPassword(char* password, int maxLength);
void saveAdminsToFile(admins *head, const char *filename);

#endif //AUTH_H
