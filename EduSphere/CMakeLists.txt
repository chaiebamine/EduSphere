cmake_minimum_required(VERSION 3.29)
project(EduSphere C)

set(CMAKE_C_STANDARD 23)

add_executable(EduSphere code/main.c
        code/ansi_codes.h
        code/main_interface.c
        code/main_interface.h
        code/student/student_interface.c
        code/student/student_interface.h
        code/teacher/teacher_interface.c
        code/utilities/utilities.c
        code/teacher/teacher_interface.h
        code/utilities/utilities.h
        code/notifications/notification.c
        code/notifications/notification.h
        code/authentification/auth.c
        code/authentification/auth.h
        code/attendence/attendence.c
        code/attendence/attendence.h
        code/admins/admin_interface.c
        code/admins/admin_interface.h
        code/authentification/auth.c
        code/Eliminations/eliminations.c
        code/Eliminations/eliminations.h
        code/student/student_interface.c
        code/student/student_interface.c
)
target_link_libraries(EduSphere winmm)
