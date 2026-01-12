#ifndef STUDENT_H
#define STUDENT_H

int student_login(char *name_buffer, int size);
void student_menu(const char *studentID);
void view_student_courses(const char *studentID);
void view_student_marks(const char *studentID);
void view_quiz_result(const char *studentID);
void student_change_password(const char *studentID);


#endif

