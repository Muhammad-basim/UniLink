#ifndef TEACHER_H
#define TEACHER_H

#define MAX_TEACHER_COURSES 4

int teacher_login(char *name_buffer, int size);
void teacher_menu(void);

void view_my_assigned_courses(const char *teacherID);
void view_students_in_course(const char *teacherID);
void change_teacher_password(const char *teacherID);
int load_teacher_courses(const char *teacherID, char courses[][20]);
void create_quiz(const char *teacherID);
// future: upload_marks(), view_marks(), etc.

extern char CURRENT_TEACHER_ID[20];

#endif

