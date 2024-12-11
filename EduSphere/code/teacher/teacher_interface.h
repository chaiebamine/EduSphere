#ifndef TEACHER_INTERFACE_H
#define TEACHER_INTERFACE_H

void teacher_ui(teacher logged_in_teacher,teachers* teachers,students* students);
void sendNotification(const char* studentCIN,const char *message);

#endif //TEACHER_INTERFACE_H
